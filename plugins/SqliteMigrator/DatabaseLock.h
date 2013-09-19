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

#include <QObject>
#include "SqliteMigrator.h"
#include <QUuid>
#include <QString>

namespace MigrationExecution {
class MigrationExecutionContext;
}

namespace SqliteMigrator {

class RefreshLockIsLivingInvoker;

/*!
 * \brief   The DatabaseLock trys to lock a database for QSqlMigritor processes.
 *          You have to check if you got the lock ( see example )
 *
 * unsigned int time_out_to_get_lock = 60;
 * context c;
 * DatabaseLock lock(c, time_out_to_get_lock);
 * if(lock) {
 *    // do mirgration stuff
 * }
 *
 */
class SQLITEMIGRATOR_DLL_EXPORT DatabaseLock : public QObject
{
    Q_OBJECT

    // TODO what are good default times?
    enum IN_SEC
    {
        tryInterval = 1,
        timeOut = 10,
        otherLockIsOutOfDateAfter = 4,
        refreshLockIsLivingInterval = 1
    };

public:
    /*! try to make a unique lock for the database in the context */
    DatabaseLock(MigrationExecution::MigrationExecutionContext &context, unsigned int timeOutTryGetLock = timeOut);

    /*! is successful locked */
    bool isExclusiveLocked() const;
    operator bool () const;

    /*! auto release lock */
    ~DatabaseLock();

    static QString buildLockFileName(const MigrationExecution::MigrationExecutionContext &context);

private:
    bool makeWaitForLock() const;
    bool isCurrentlyLocked() const;
    bool tryReleaseOutOfDateLock() const;
    bool makeLock() const;
    bool writeUuuid() const;
    bool tryReleaseOwnLock() const;
    bool releaseLock() const;

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
