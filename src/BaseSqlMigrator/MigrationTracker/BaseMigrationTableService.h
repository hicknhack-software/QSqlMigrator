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
#ifndef MIGRATIONTRACKER_BASEMIGRATIONTABLESERVICE_H
#define MIGRATIONTRACKER_BASEMIGRATIONTABLESERVICE_H

#include "config.h"

#include "MigrationTracker/MigrationTrackerService.h"

namespace MigrationTracker {

/*!
 * \brief Implementation of the successfully executed migration tracking through a databse table
 */
class QSQLMIGRATOR_DLL_EXPORT BaseMigrationTableService : public MigrationTracker::MigrationTrackerService
{
public:
    ~BaseMigrationTableService() QSQL_OVERRIDE_D {}

    bool canRevertStrucuturalChangesUsingTransactions() const Q_DECL_OVERRIDE;

    bool wasMigrationExecuted(const QString &migrationName,
                              const CommandExecution::CommandExecutionContext &context) const Q_DECL_OVERRIDE;

    QStringList migrationList(const CommandExecution::CommandExecutionContext &context) const Q_DECL_OVERRIDE;

    bool addMigration(const QString &migrationName,
                      const CommandExecution::CommandExecutionContext &context) const Q_DECL_OVERRIDE;

    bool removeMigration(const QString &migrationName,
                         const CommandExecution::CommandExecutionContext &context) const Q_DECL_OVERRIDE;

    bool prepare(const MigrationExecution::MigrationExecutionContext &context) const Q_DECL_OVERRIDE;
};

} // namespace MigrationTracker

#endif // MIGRATIONTRACKER_BASEMIGRATIONTABLESERVICE_H
