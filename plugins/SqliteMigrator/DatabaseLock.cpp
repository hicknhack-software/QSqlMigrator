/****************************************************************************
**
** Copyright (C) 2013, HicknHack Software
** All rights reserved.
** Contact: http://www.hicknhack-software.com/contact
**
** This file is part of the QSqlMigrator
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/
#include "DatabaseLock.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>
#include <QThread>

#include <functional>

#ifdef Q_OS_WIN
#include <windows.h> // Sleep
#define SleepSec(sec) Sleep(sec * 1000)
#else
#include <unistd.h> // sleep
#define SleepSec(sec) usleep(sec * 1000 * 1000)
#endif

namespace SqliteMigrator {

class IntervallCallback : public QObject {
    Q_OBJECT
public:
    using IntervalSeconds = int;
    using Callback = std::function<void()>;

public:
    IntervallCallback(IntervalSeconds, Callback);
    ~IntervallCallback();

private:
    void run();

protected slots:
    void update();

private:
    Callback m_callback;
    QThread m_thread;
    QTimer m_timer;
};

IntervallCallback::IntervallCallback(IntervalSeconds intervalSeconds, Callback callback)
    : m_callback(callback)
{
    m_timer.setSingleShot(false);
    m_timer.setInterval(intervalSeconds * 1000);
    m_timer.moveToThread(&m_thread);
    this->moveToThread(&m_thread);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(&m_thread, SIGNAL(started()), &m_timer, SLOT(start()));
    connect(&m_thread, SIGNAL(finished()), &m_timer, SLOT(stop()));
    m_thread.start();
}

IntervallCallback::~IntervallCallback()
{
    m_thread.quit();
    m_thread.wait();
}

void
IntervallCallback::update()
{
    m_callback();
}

DatabaseLock::DatabaseLock(QSqlDatabase database, Config config)
    : QObject(Q_NULLPTR)
    , m_config(config)
    , m_uuid(QUuid::createUuid())
    , m_lockFileName(buildLockFileName(database))
{
    m_lockedSuccessful = tryMakeWaitForLock();

    if (m_lockedSuccessful) {
        m_refreshLockInterval.reset(
            new IntervallCallback(m_config.refreshLockInterval, [&]() { refreshLockIsLiving(); }));
    }
}

QString
DatabaseLock::buildLockFileName(const QSqlDatabase &database)
{
    static const QString lockFileNameExtension = ".qsqlm_lock";
    return database.databaseName() + lockFileNameExtension;
}

QString
DatabaseLock::ownProcessInfo() const
{
    static const QString pid = "PID:";
    return pid + m_uuid.toString();
}

bool
DatabaseLock::tryMakeWaitForLock() const
{
    for (unsigned int i = 0; i != m_config.tryGetLockCount; ++i) {
        if (tryReleaseOutOfDateLock())
            return tryMakeLock();

        SleepSec(m_config.tryGetLockInterval);
    }

    ::qWarning() << ownProcessInfo() << "can NOT lock db or release the existing lock";
    ::qWarning() << "TIMEOUT: an other running process have currently the lock:" << m_lockFileName;
    return false;
}

bool
DatabaseLock::isCurrentlyLocked() const
{
    return QFile::exists(m_lockFileName);
}

bool
DatabaseLock::tryReleaseOutOfDateLock() const
{
    if (!isCurrentlyLocked())
        return true;

    QDateTime lastModified = QFileInfo(m_lockFileName).lastModified();
    ::qDebug() << ownProcessInfo() << "current lock last update" << lastModified;
    if (lastModified.addSecs(m_config.otherLockIsOutOfDateAfter) < QDateTime::currentDateTime()) {
        ::qWarning() << ownProcessInfo() << "the lock is out of date";
        ::qWarning() << "NOTE: The last job has NOT finished correctly!" << m_lockFileName;
        return tryReleaseAnyLock();
    }
    return false;
}

bool
DatabaseLock::tryMakeLock() const
{
    if (isCurrentlyLocked()) {
        ::qDebug() << ownProcessInfo() << "can NOT lock db, db is currently locked!";
        return false;
    }

    if (!tryWriteUuidToLockFile()) {
        return false;
    }

    ::qDebug() << ownProcessInfo() << "get lock database:" << m_lockFileName;
    return true;
}

bool
DatabaseLock::tryWriteUuidToLockFile() const
{
    QFile lockFile(m_lockFileName);
    if (!lockFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        ::qWarning() << ownProcessInfo() << "can NOT lock db, can NOT create lock file!";
        return false;
    }

    if (!lockFile.write(m_uuid.toByteArray())) {
        ::qWarning() << ownProcessInfo() << "can NOT lock db, can NOT write UUID to lock file!";
        return false;
    }
    lockFile.close();

    return true;
}

DatabaseLock::~DatabaseLock()
{
    // do not release the lock before killing
    // the callback might create a new lock!
    m_refreshLockInterval.reset();
    tryReleaseOwnLock();
}

bool
DatabaseLock::tryReleaseOwnLock() const
{
    if (!isCurrentlyLocked()) {
        ::qDebug() << ownProcessInfo() << "can NOT release lock, db is NOT locked!";
        return false;
    }

    QFile lockFile(m_lockFileName);
    if (!lockFile.open(QIODevice::ReadOnly)) {
        ::qWarning() << ownProcessInfo() << "can NOT release lock, can NOT open existing lock file!"
                     << m_lockFileName;
        return false;
    }

    if (m_uuid.toString() != lockFile.readLine()) {
        ::qDebug() << ownProcessInfo() << "can NOT release own lock, db is NOT locked from me!";
        lockFile.close();
        return false;
    }
    lockFile.close();

    return tryReleaseAnyLock();
}

bool
DatabaseLock::tryReleaseAnyLock() const
{
    if (!QFile::remove(m_lockFileName)) {
        ::qWarning() << ownProcessInfo() << "can NOT release any lock, can NOT delete lock file!"
                     << m_lockFileName;
        return false;
    }

    ::qDebug() << ownProcessInfo() << "lock released:" << m_lockFileName;
    return true;
}

bool
DatabaseLock::refreshLockIsLiving() const
{
    return tryWriteUuidToLockFile();
}

void
DatabaseLock::IntervallCallbackDeleter::cleanup(IntervallCallback *intervalCallback)
{
    delete intervalCallback;
}

} // namespace SqliteMigrator

#include "DatabaseLock.moc"
