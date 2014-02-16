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
#ifndef DATABASELOCK_H
#define DATABASELOCK_H

#include "CommandExecution/CommandExecutionServiceRepository.h"
#include "MigrationTracker/MigrationTrackerService.h"

#include <QObject>
#include "SqliteMigrator.h"
#include <QUuid>
#include <QString>

namespace MigrationExecution {
class MigrationExecutionContext;
}

namespace SqliteMigrator {

class RefreshLockIsLivingInvoker;

// TODO:
// P1: what are good default time values?
// P2: do the user need the option to set a own timeout?

/*!
 * \brief   The DatabaseLock try to lock a database for this QSqlMigrator process.
 *          You have to check if you got the lock ( see example with operator bool ).
 *
 *          The lock will be done with a lock-file in the same folder where the db
 *          is locatded.
 *
 *          The lock will automatically released on deconstruction.
 *
 * Example:
 *
 * unsigned int timeOutToGetLock = 60;
 * context c( ... );
 * DatabaseLock lock(c, timeOutToGetLock);
 * if(lock) {
 *    // do mirgration stuff
 * }
 *
 */
class SQLITEMIGRATOR_DLL_EXPORT DatabaseLock : public QObject
{
    Q_OBJECT

    // P1: what are good default time values?
    enum IN_SEC
    {
        tryGetLockInterval = 1,
        timeOut = 10,
        otherLockIsOutOfDateAfter = 4,
        refreshLockIsLivingInterval = 1
    };

public:
    /*! try to make a lock for the database in the context */                         // P2: do the user need the option to set a own timeout?
    DatabaseLock(MigrationExecution::MigrationExecutionContext &context, unsigned int timeOutTryGetLock = timeOut);

    /*! return true if the lock is successfully set for this process */
    operator bool () const;

    /*! auto release the own lock */
    ~DatabaseLock();

    /*! return the lock file name which will be used for this migration context */
    static QString buildLockFileName(const MigrationExecution::MigrationExecutionContext &context);

private:
    QString ownProcessInfo() const;

    bool tryMakeWaitForLock() const;
    bool isCurrentlyLocked() const;
    bool tryReleaseOutOfDateLock() const;
    bool tryMakeLock() const;
    bool tryWriteUuidToLockFile() const;
    bool tryReleaseOwnLock() const;
    bool tryReleaseAnyLock() const;


    const QUuid m_uuid;
    const QString m_lockFileName;
    const unsigned int m_timeOutTryGetLock;
    bool m_lockedSuccessful;


    RefreshLockIsLivingInvoker* m_refreshLockIsLivingInvoker;
    friend class RefreshLockIsLivingInvoker;
private slots:
    bool refreshLockIsLiving() const;
};

} // namespace SqliteMigrator

#endif // DATABASELOCK_H
