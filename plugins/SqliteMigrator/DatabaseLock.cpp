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

#include <QSqlDatabase>
#include <QDebug>
#include <QFile>
#include <QUuid>


#ifdef Q_OS_WIN
#include <windows.h> // Sleep
#else
#include <unistd.h>  // usleep
void Sleep(unsigned int sec)
{
    usleep(sec * 1000 * 1000);
}
#endif


namespace SqliteMigrator {


DatabaseLock::DatabaseLock(MigrationExecution::MigrationExecutionContext &context, unsigned int timeOutInSec)
    : m_uuid(QUuid::createUuid()),
      m_lockFileName(buildLockFileName(context)),
      m_timeOutInSec(timeOutInSec),
      m_lockedSuccessful(waitForLock())
{}

bool DatabaseLock::isExclusiveLocked() const
{
    return m_lockedSuccessful;
}

bool DatabaseLock::waitForLock() const
{
    unsigned int i = 0;
    while(isStillLocked()) {
        Sleep(check_interval_in_sec);
        ::qWarning() << m_uuid <<"tryed to lock, but database is locked";

        if(time_out == i)
            return false;
        ++i;
    }

    return makeLock();
}

QString DatabaseLock::buildLockFileName(const MigrationExecution::MigrationExecutionContext &context)
{
    QString migrationLockFileName = context.database().databaseName();
    migrationLockFileName += ".qsqlm_lock";

    return migrationLockFileName;
}

bool DatabaseLock::isStillLocked() const
{
    if(QFile::exists(m_lockFileName))
        return true;

    return false;
}

bool DatabaseLock::makeLock() const
{
    if(isStillLocked()) {
        ::qDebug() << m_uuid << "can NOT lock db, db is currently locked!";
        return false;
    }

    QFile lockFile(m_lockFileName);
    if (!lockFile.open(QIODevice::WriteOnly)) {
        ::qDebug() << m_uuid << "can NOT lock db, can NOT create lock file!";
        return false;
    }
    lockFile.write(m_uuid.toByteArray());
    lockFile.close();

    if(!QFile::exists(m_lockFileName)) {
        ::qDebug() << m_uuid << "can NOT lock db, can NOT create lock file!";
        return false;
    }

    ::qDebug() << m_uuid << "Lock database:" << m_lockFileName;

    return true;
}

DatabaseLock::~DatabaseLock()
{
    releaseLock();
}

bool DatabaseLock::releaseLock() const
{
    if(!isStillLocked()) {
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

    if(!QFile::remove(m_lockFileName)) {
        ::qDebug() << m_uuid << "can NOT release lock - can NOT delete lock file!";
        return false;
    }

    return true;
}

}
