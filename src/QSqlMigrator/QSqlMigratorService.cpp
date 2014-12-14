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
#include "QSqlMigratorService.h"

#include "CommandExecution/CommandExecutionContext.h"
#include "Migrations/MigrationRepository.h"
#include "MigrationExecution/MigrationExecutionContext.h"
#include "MigrationExecution/MigrationExecutionService.h"
#include "MigrationTracker/MigrationTrackerService.h"

using namespace MigrationExecution;

namespace QSqlMigrator {

QStringList QSqlMigratorService::appliedMigrations() const
{
    CommandExecution::CommandExecutionContext serviceContext(context.database(), context.migrationConfig(), context.helperRepository());
    QStringList list = context.migrationTrackerService()->migrationList(serviceContext);
    list.sort();
    return list;
}

bool QSqlMigratorService::applyAll() const
{
    const MigrationExecutionService migrator;
    return migrator.executeBatch(this->unappliedMigrations(), context);
}

bool QSqlMigratorService::applyMigration(const QString &migrationName) const
{
    const MigrationExecutionService migrator;
    return migrator.execute(migrationName, context);
}

bool QSqlMigratorService::migrateTo(const QString &migrationName) const
{
    if (migrationName == this->lastAppliedMigration()) {
        return true;
    }

    bool bSuccess = true;
    const MigrationExecutionService migrator;
    while (bSuccess && (this->lastAppliedMigration() != migrationName)) {
        if (migrationName < this->lastAppliedMigration()) {
            bSuccess = migrator.execute(this->lastAppliedMigration(), context, MigrationExecutionService::Down);
        } else {
            QStringList unapplied = this->unappliedMigrations();
            if(!unapplied.empty())
                bSuccess = migrator.execute(unapplied.first(), context, MigrationExecutionService::Up);
        }
    }
    return bSuccess;
}

bool QSqlMigratorService::revertMigration(const QString &migrationName) const
{
    const MigrationExecutionService migrator;
    return migrator.execute(migrationName, context, MigrationExecutionService::Down);
}

QStringList QSqlMigratorService::definedMigrations() const
{
    QStringList list = context.migrationMap().keys();
    list.sort();
    return list;
}

QString QSqlMigratorService::lastAppliedMigration() const
{
    const QStringList appliedMigrations = this->appliedMigrations();
    if (appliedMigrations.isEmpty()) {
        return QString("");
    } else {
        return appliedMigrations.last();
    }
}

QStringList QSqlMigratorService::missingMigrations() const
{
    //applied - defined
    QStringList missingMigrations;
    foreach (QString migrationName, this->appliedMigrations()) {
        if (!this->definedMigrations().contains(migrationName)) {
            missingMigrations << migrationName;
        }
    }
    missingMigrations.sort();
    return missingMigrations;
}

QStringList QSqlMigratorService::unappliedMigrations() const
{
    //defined - applied
    QStringList unappliedMigrations;
    foreach (QString migrationName, this->definedMigrations()) {
        if (!this->appliedMigrations().contains(migrationName)) {
            unappliedMigrations << migrationName;
        }
    }
    unappliedMigrations.sort();
    return unappliedMigrations;
}

} // namespace QSqlMigrator
