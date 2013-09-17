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
#ifndef MIGRATIONTRACKER_SQLITEMIGRATIONTABLESERVICE_H
#define MIGRATIONTRACKER_SQLITEMIGRATIONTABLESERVICE_H

#include "MigrationTracker/BaseMigrationTrackerService.h"
#include "SqliteMigrator/SqliteMigrator.h"

namespace MigrationTracker {

class SQLITEMIGRATOR_DLL_EXPORT SqliteMigrationTableService : public MigrationTracker::BaseMigrationTrackerService
{
public:
    SqliteMigrationTableService();

    bool canRevertStrucuturalChangesUsingTransactions() const;
    bool wasMigrationExecuted(const QString &migrationName
                                      , const CommandExecution::CommandExecutionContext &context) const;
    QStringList migrationList(const CommandExecution::CommandExecutionContext &context) const;

    bool addMigration(const QString &migrationName, const CommandExecution::CommandExecutionContext &context) const;
    bool removeMigration(const QString &migrationName, const CommandExecution::CommandExecutionContext &context) const;

    bool ensureVersionTable(const MigrationExecution::MigrationExecutionContext &context) const;

    bool isMigrationLocked(const MigrationExecution::MigrationExecutionContext &) const;
    bool makeExclusiveMigrationLock(const MigrationExecution::MigrationExecutionContext &) const;
    bool releaseExclusiveMigrationLock(const MigrationExecution::MigrationExecutionContext &) const;
};

} // namespace MigrationTracker

#endif // MIGRATIONTRACKER_SQLITEMIGRATIONTABLESERVICE_H
