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
#include "MigrationExecutionService.h"

#include "Commands/BaseCommand.h"
#include "Commands/CreateTable.h"
#include "CommandExecution/BaseCommandExecutionService.h"
#include "CommandExecution/CommandExecutionContext.h"
#include "CommandExecution/CommandExecutionServiceRepository.h"
#include "Migrations/Migration.h"
#include "MigrationTracker/BaseMigrationTrackerService.h"
#include "QSqlMigrator/QSqlMigratorConfig.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtCore>
#include <Qt>


using namespace Commands;

namespace MigrationExecution {

MigrationExecutionService::MigrationExecutionService()
{
}

CommandPtrList reverseCommands(const CommandPtrList &commands)
{
    CommandPtrList reversedCommands;
    reversedCommands.reserve( commands.size() );
    foreach (Commands::CommandPtr command, commands) {
        if (!command) {
            ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "command is 0!";
            return CommandPtrList();
        }

        Commands::CommandPtr reversed = command->reverse();
        if (reversed.isNull()) {
            return CommandPtrList();
        }
        reversedCommands.prepend(reversed);
    }

    return reversedCommands;
}

bool MigrationExecutionService::execute(const QString &migrationName
                                        , const MigrationExecutionContext &migrationContext
                                        , Direction direction) const
{
    if (this->isMigrationRemembered(migrationName, migrationContext, direction)) {
        return true; // everything ok
    }

    QSqlDatabase database = migrationContext.database();

    const MigrationExecutionConfig &migrationConfig = migrationContext.migrationConfig();
    CommandExecution::CommandExecutionContext context(database, migrationConfig, migrationContext.helperRepository());
    CommandPtrList undoCommands;
    if(migrationName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "migrationName is empty";
        return false;
    }

    CommandPtrList migrationCommands = migrationContext.migrationMap()[migrationName]->commandList();
    if( migrationCommands.isEmpty() ) {
        ::qWarning() << LOG_PREFIX << "no comands for migration";
        return false;
    }

    if (direction == Down) {
        migrationCommands = reverseCommands(migrationCommands);
        if( migrationCommands.isEmpty() ) {
            ::qWarning() << LOG_PREFIX << "cannot reverse migration commands";
            return false;
        }
    }

    MigrationTableServicePtr migrationTableService = migrationContext.baseMigrationTableService();
    if(!migrationTableService) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "migrationTableService is 0";
        return false;
    }

    bool haveTransaction = false;
    if (migrationTableService->canRevertStrucuturalChangesUsingTransactions()) {
        if (!database.transaction()) {
            ::qWarning() << LOG_PREFIX << "starting transaction failed";
            ::qWarning() << LOG_PREFIX << database.lastError();
            return false; // no transaction
        }
        haveTransaction = true;
    } else {
        context.setIsUndoUsed(true);
        undoCommands.reserve(migrationCommands.size());
    }

    ::qDebug() << LOG_PREFIX << "running Migration: " << migrationName;
    CommandServiceRepositoryPtr commandServiceRepository = migrationContext.commandServiceRepository();
    bool isSuccess = m_execution.batch(migrationCommands, undoCommands, commandServiceRepository, context);
    if (isSuccess) {
        this->rememberMigration(migrationName, migrationContext, direction);

        if(haveTransaction) {
            return database.commit();
        }
        return true;
    }

    if (haveTransaction) {
        database.rollback();
    } else if (context.isUndoUsed()) {
        context.setIsUndoUsed(false);
        m_execution.batch(undoCommands, undoCommands, commandServiceRepository, context);
    }
    return false;
}

bool MigrationExecutionService::executeBatch(const QStringList &migrationList
                                             , const MigrationExecutionContext &context
                                             , Direction direction) const
{
    if (migrationList.empty()) {
        return true; // No migrations present, no need to do anything
    }

    bool success = true;
    foreach (const QString &migrationName, migrationList) {
        success &= this->execute(migrationName, context, direction);
        if (!success) {
            break;
        }
    }
    return success;
}

bool MigrationExecutionService::isMigrationRemembered(const QString &migrationName
                                                      , const MigrationExecutionContext &context
                                                      , Direction direction) const
{
    CommandExecution::CommandExecutionContext serviceContext(context.database(), context.migrationConfig(), context.helperRepository());
    MigrationTableServicePtr tableService = context.baseMigrationTableService();
    bool isExecuted = tableService->wasMigrationExecuted(migrationName, serviceContext);

    switch (direction) {
    case Up:
        return isExecuted;

    case Down:
        return (!isExecuted);

    default:
        return true;
    }
}

bool MigrationExecutionService::rememberMigration(const QString &migrationName
                                                  , const MigrationExecutionContext &context
                                                  , Direction direction) const
{
    CommandExecution::CommandExecutionContext serviceContext(context.database(), context.migrationConfig(), context.helperRepository());
    MigrationTableServicePtr tableService = context.baseMigrationTableService();
    switch (direction) {
    case Up:
        return tableService->addMigration(migrationName, serviceContext);

    case Down:
        return tableService->removeMigration(migrationName, serviceContext);

    default:
        return false;
    }
}

} //namespace MigrationExecution
