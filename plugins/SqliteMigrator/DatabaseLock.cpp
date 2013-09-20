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
    /*! init with the database lock which should invoked */
    RefreshLockIsLivingInvoker(const DatabaseLock*const databaseLock);

    /*! start the invoke timer for x seconds */
    void start(unsigned int invokeAfterSecs);

    /*! safety stop the timer and the thread, but you can use the dctor as well */
    void stop();

    /*! safety auto clean */
    ~RefreshLockIsLivingInvoker();

private:
    void run();

    const DatabaseLock*const m_databaseLock;
    unsigned int m_sleepFor;
    bool m_isStopped;
};

DatabaseLock::DatabaseLock(MigrationExecution::MigrationExecutionContext &context, unsigned int timeOutTryGetLock)
    : QObject(NULL),
      m_uuid(QUuid::createUuid()),
      m_lockFileName(buildLockFileName(context)),
      m_timeOutTryGetLock(timeOutTryGetLock),
      m_lockedSuccessful(false),
      m_refreshLockIsLivingInvoker(NULL)
{
    m_lockedSuccessful= tryMakeWaitForLock();

    if(m_lockedSuccessful) {
        m_refreshLockIsLivingInvoker = new RefreshLockIsLivingInvoker(this);
        m_refreshLockIsLivingInvoker->start(refreshLockIsLivingInterval);
    }
}

SqliteMigrator::DatabaseLock::operator bool() const
{
    return m_lockedSuccessful;
}

QString DatabaseLock::buildLockFileName(const MigrationExecution::MigrationExecutionContext &context)
{
    static const QString lockFileNameExtension = ".qsqlm_lock";
    return context.database().databaseName() + lockFileNameExtension;
}

QString DatabaseLock::ownProcessInfo() const
{
    static const QString pid = "PID:";
    return pid + m_uuid;
}

bool DatabaseLock::tryMakeWaitForLock() const
{
    for(unsigned int i = 0 ; i != m_timeOutTryGetLock ; ++i ) {
        if(!isCurrentlyLocked())
            return tryMakeLock();

        if(tryReleaseOutOfDateLock())
            return tryMakeLock();

        SleepSec(tryGetLockInterval);
    }

    ::qWarning() << ownProcessInfo() << "can NOT lock db or release the existing lock";
    ::qWarning() << "TIMEOUT: an other running process have currently the lock:" << m_lockFileName;
    return false;
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
    ::qDebug() << ownProcessInfo() << "current lock last update" << lastModified;
    if(lastModified.addSecs(otherLockIsOutOfDateAfter) < QDateTime::currentDateTime()) {
        ::qWarning() << ownProcessInfo() << "the lock is out of date";
        ::qWarning() << "NOTE: anywhere is may be a unfinished or broken job!" << m_lockFileName;
        return tryReleaseAnyLock();
    }

    return false;
}

bool DatabaseLock::tryMakeLock() const
{
    if(isCurrentlyLocked()) {
        ::qDebug() << ownProcessInfo() << "can NOT lock db, db is currently locked!";
        return false;
    }

    if(!tryWriteUuidToLockFile()){
        return false;
    }

    ::qDebug() << ownProcessInfo() << "get lock database:" << m_lockFileName;

    return true;
}

bool DatabaseLock::tryWriteUuidToLockFile() const
{
    QFile lockFile(m_lockFileName);
    if (!lockFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        ::qWarning() << ownProcessInfo() << "can NOT lock db, can NOT create lock file!";
        return false;
    }

    if(!lockFile.write(m_uuid.toByteArray())) {
        ::qWarning() << ownProcessInfo() << "can NOT lock db, can NOT write UUID to lock file!";
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
        ::qDebug() << ownProcessInfo() << "can NOT release lock, db is NOT locked!";
        return false;
    }

    QFile lockFile(m_lockFileName);
    if (!lockFile.open(QIODevice::ReadOnly)) {
        ::qWarning() << ownProcessInfo() << "can NOT release lock, can NOT open existing lock file!" << m_lockFileName;
        return false;
    }

    if(m_uuid.toString() != lockFile.readLine()) {
        ::qDebug() << ownProcessInfo() << "can NOT release own lock, db is NOT locked from me!";
        lockFile.close();
        return false;
    }
    lockFile.close();

    return tryReleaseAnyLock();
}

bool DatabaseLock::tryReleaseAnyLock() const
{
    if(!QFile::remove(m_lockFileName)) {
        ::qWarning() << ownProcessInfo() << "can NOT release any lock, can NOT delete lock file!" << m_lockFileName;
        return false;
    }

    ::qDebug() << ownProcessInfo() << "lock released:" << m_lockFileName;
    return true;
}

bool DatabaseLock::refreshLockIsLiving() const
{
    return tryWriteUuidToLockFile();
}

/**
 * impl RefreshLockIsLivingInvoker
 **/
RefreshLockIsLivingInvoker::RefreshLockIsLivingInvoker(const DatabaseLock*const databaseLock)
    : m_databaseLock(databaseLock),
      m_sleepFor(0),
      m_isStopped(false)
{}

void RefreshLockIsLivingInvoker::start(unsigned int callbackAfterSecs)
{
    m_sleepFor = callbackAfterSecs;
    QThread::start();
}

RefreshLockIsLivingInvoker::~RefreshLockIsLivingInvoker()
{
    this->stop();
}

void RefreshLockIsLivingInvoker::stop()
{
    m_isStopped = true;
    QThread::quit();
    QThread::wait();
}

void RefreshLockIsLivingInvoker::run()
{
    while(!m_isStopped) {
        QThread::sleep(m_sleepFor);
        m_databaseLock->refreshLockIsLiving();
    }
}

}
