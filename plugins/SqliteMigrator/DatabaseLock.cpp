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

#include "MigrationExecution/MigrationExecutionContext.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>
#include <QThread>


#ifdef Q_OS_WIN
#include <windows.h> // Sleep
#define SleepSec(sec) Sleep(sec * 1000)
#else
#include <unistd.h>  // sleep
#define SleepSec(sec) usleep(sec * 1000 * 1000)
#endif


namespace SqliteMigrator {

class RefreshLockIsLivingInvoker : public QThread
{
public:
    /*! init with the database lock wich should invoked */
    RefreshLockIsLivingInvoker(DatabaseLock*const databaseLock);
    /*! safety auto clean */
    ~RefreshLockIsLivingInvoker();

    /*! start the timer for x seconds */
    void start(unsigned int invokeAfterSecs);
    /*! safety stop the timer and the thread */
    void stop();

private:
    void run();

    DatabaseLock*const m_databaseLock;
    unsigned int m_sleepFor;
    bool isStopped;
};

DatabaseLock::DatabaseLock(MigrationExecution::MigrationExecutionContext &context, unsigned int timeOutTryGetLock)
    : QObject(NULL),
      m_uuid(QUuid::createUuid()),
      m_lockFileName(buildLockFileName(context)),
      m_timeOutTryGetLock(timeOutTryGetLock),
      m_lockedSuccessful(false),
      m_refreshLockIsLivingInvoker(NULL)
{
    m_lockedSuccessful= makeWaitForLock();

    if(m_lockedSuccessful) {
        m_refreshLockIsLivingInvoker = new RefreshLockIsLivingInvoker(this);
        m_refreshLockIsLivingInvoker->start(refreshLockIsLivingInterval);
    }
}

bool DatabaseLock::isExclusiveLocked() const
{
    return m_lockedSuccessful;
}

SqliteMigrator::DatabaseLock::operator bool() const
{
    return isExclusiveLocked();
}

QString DatabaseLock::buildLockFileName(const MigrationExecution::MigrationExecutionContext &context)
{
    QString migrationLockFileName = context.database().databaseName();
    migrationLockFileName += lockFileNameExtension;

    return migrationLockFileName;
}

bool DatabaseLock::makeWaitForLock() const
{
    unsigned int i = 0;
    while(isCurrentlyLocked()) {
        if(tryReleaseOutOfDateLock())
            return makeLock();

        SleepSec(tryInterval);
        ::qDebug() << m_uuid << "try to lock db, but database is locked from other process";

        if(m_timeOutTryGetLock == i) {
            ::qWarning() << m_uuid << "can NOT lock db - timeout";
            return false;
        }

        ++i;
    }

    return makeLock();
}

bool DatabaseLock::isCurrentlyLocked() const
{
    return QFile::exists(m_lockFileName);
}

bool DatabaseLock::tryReleaseOutOfDateLock() const
{
    if(!isCurrentlyLocked()) {
        return true;
    }

    QDateTime lastModified = QFileInfo(m_lockFileName).lastModified();
    ::qDebug() << "current lock last update" << lastModified;
    if(lastModified.addSecs(otherLockIsOfDateAfter) < QDateTime::currentDateTime()) {
        ::qWarning() << m_uuid << "release out of date lock";
        return releaseLock();
    }

    return false;
}

bool DatabaseLock::makeLock() const
{
    if(isCurrentlyLocked()) {
        ::qDebug() << m_uuid << "can NOT lock db, db is currently locked!";
        return false;
    }

    if(!writeUuuid()){
        return false;
    }

    ::qDebug() << m_uuid << "lock database:" << m_lockFileName;

    return true;
}

bool DatabaseLock::writeUuuid() const
{
    QFile lockFile(m_lockFileName);
    if (!lockFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        ::qDebug() << m_uuid << "can NOT lock db, can NOT create lock file!";
        return false;
    }

    if(!lockFile.write(m_uuid.toByteArray())) {
        ::qDebug() << m_uuid << "can NOT write UUID lock file!";
        return false;
    }
    lockFile.close();

    return true;
}

DatabaseLock::~DatabaseLock()
{
    // NOTE do not release the lock before killing
    //      the invoker, otherwise the invoker get the
    //      chance to create a new lock!
    if(m_refreshLockIsLivingInvoker) {
        delete m_refreshLockIsLivingInvoker;
    }

    tryReleaseOwnLock();
}

bool DatabaseLock::tryReleaseOwnLock() const
{
    if(!isCurrentlyLocked()) {
        ::qDebug() << m_uuid << "can NOT release lock - db is NOT locked!";
        return false;
    }

    QFile lockFile(m_lockFileName);
    if (!lockFile.open(QIODevice::ReadOnly)) {
        ::qDebug() << m_uuid << "can NOT release lock - can NOT open lock file!";
        return false;
    }

    if(m_uuid.toString() != lockFile.readLine()) {
        ::qDebug() << m_uuid << "can NOT release lock - db is NOT locked from me!";
        lockFile.close();
        return false;
    }
    lockFile.close();

    return releaseLock();
}

bool DatabaseLock::releaseLock() const
{
    if(!QFile::remove(m_lockFileName)) {
        ::qDebug() << m_uuid << "can NOT release lock - can NOT delete lock file!";
        return false;
    }

    ::qDebug() << m_uuid << "lock released:" << m_lockFileName;
    return true;
}

bool DatabaseLock::refreshLockIsLiving() const
{
    return writeUuuid();
}

RefreshLockIsLivingInvoker::RefreshLockIsLivingInvoker(DatabaseLock*const databaseLock)
    : m_databaseLock(databaseLock),
      m_sleepFor(0),
      isStopped(false)
{}

void RefreshLockIsLivingInvoker::start(unsigned int callbackAfterSecs)
{
    m_sleepFor = callbackAfterSecs;
    QThread::start();
}

RefreshLockIsLivingInvoker::~RefreshLockIsLivingInvoker()
{
    stop();
}

void RefreshLockIsLivingInvoker::stop()
{
    isStopped = true;
    QThread::quit();
    QThread::wait();
}

void RefreshLockIsLivingInvoker::run()
{
    while(!isStopped) {
        QThread::sleep(m_sleepFor);
        m_databaseLock->refreshLockIsLiving();
    }
}

}
