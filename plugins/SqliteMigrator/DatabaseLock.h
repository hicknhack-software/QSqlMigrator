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
#pragma once

#include "SqliteMigrator.h"

#include <QObject>
#include <QUuid>
#include <QString>

namespace SqliteMigrator {

class IntervallCallback;

/*!
 * \brief The DatabaseLock try to lock a database for this QSqlMigrator process.
 * You have to check if you got the lock ( see example with operator bool ).
 *
 * The lock will be done with a lock-file in the same folder where the db is locatded.
 *
 * The lock will automatically released on deconstruction.
 *
 * Example:
 *
 * DatabaseLock lock(database);
 * if(lock) {
 *    // do mirgration stuff
 * }
 *
 */
class SQLITEMIGRATOR_DLL_EXPORT DatabaseLock : public QObject {
    Q_OBJECT

public:
    struct Config {
        Config () : tryGetLockInterval(1), tryGetLockCount(10), otherLockIsOutOfDateAfter(4), refreshLockInterval(1) {};
        int tryGetLockInterval;
        int tryGetLockCount;
        int otherLockIsOutOfDateAfter;
        int refreshLockInterval;
    };

public:
    /// try to make a lock for the database in the context
    DatabaseLock(QSqlDatabase, Config = Config());

    /// auto release the own lock
    ~DatabaseLock();

    /// return true if the lock is successfully set for this process
    operator bool() const;

    /// return the lock file name which will be used for this migration context
    static QString buildLockFileName(const QSqlDatabase &);

private:
    QString ownProcessInfo() const;

    bool tryMakeWaitForLock() const;
    bool isCurrentlyLocked() const;
    bool tryReleaseOutOfDateLock() const;
    bool tryMakeLock() const;
    bool tryWriteUuidToLockFile() const;
    bool tryReleaseOwnLock() const;
    bool tryReleaseAnyLock() const;
    bool refreshLockIsLiving() const;

private:
    Config m_config;
    const QUuid m_uuid;
    const QString m_lockFileName;
    bool m_lockedSuccessful = false;

    struct IntervallCallbackDeleter {
        static void cleanup(IntervallCallback*);
    };
    QScopedPointer<IntervallCallback, IntervallCallbackDeleter> m_refreshLockInterval;
};

inline DatabaseLock::operator bool() const
{
    return m_lockedSuccessful;
}

} // namespace SqliteMigrator
