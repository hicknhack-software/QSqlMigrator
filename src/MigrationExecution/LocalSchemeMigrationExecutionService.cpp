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
#include "LocalSchemeMigrationExecutionService.h"

#include "Migrations/Migration.h"
#include "QSqlMigrator/QSqlMigratorConfig.h"

#include <QStringList>
#include <QDebug>

using namespace Commands;

namespace MigrationExecution {

LocalSchemeMigrationExecutionService::LocalSchemeMigrationExecutionService()
{
}

LocalSchemeMigrationExecutionService::~LocalSchemeMigrationExecutionService()
{
}

bool LocalSchemeMigrationExecutionService::execute(const QString &migrationName,
                                                   LocalSchemeMigrationExecutionContext &migrationContext) const
{
    LocalSchemePtr localScheme = migrationContext.localScheme();

    CommandExecution::LocalSchemeCommandExecutionContext context(localScheme);
    if(migrationName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "migrationName is empty";
        return false;
    }

    CommandPtrList migrationCommands = migrationContext.migrationMap()[migrationName]->commandList();
    if( migrationCommands.isEmpty() ) {
        ::qWarning() << LOG_PREFIX << "no commands for migration";
        return false;
    }

    ::qDebug() << LOG_PREFIX << "running Migration on local scheme: " << migrationName;

    LocalSchemeCommandServiceRepositoryPtr commandServiceRepository = migrationContext.localSchemeCommandServiceRepository();
    bool isSuccess = m_execution.batch(migrationCommands, commandServiceRepository, context);

    return isSuccess;
}

bool LocalSchemeMigrationExecutionService::executeBatch(const QStringList &migrationList,
                                                        LocalSchemeMigrationExecutionContext &migrationContext) const
{
    if (migrationList.empty()) {
        return true; // No migrations present, no need to do anything
    }

    bool success = true;
    foreach (const QString &migrationName, migrationList) {
        success &= this->execute(migrationName, migrationContext);
        if (!success) {
            break;
        }
    }
    return success;
}

} //namespace MigrationExecution
