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

using namespace Commands;

namespace MigrationExecution {

MigrationExecutionService::MigrationExecutionService()
{
}

bool MigrationExecutionService::execute(const QString &migrationName
                                        , const MigrationExecutionContext &migrationContext
                                        , Direction direction) const
{
    if (this->isMigrationRemembered(migrationName, migrationContext, direction)) {
        return true; // everything ok
    }

    DatabasePtr database = migrationContext.database();
    if(!database) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "database is 0!";
        return false;
    }

    const MigrationExecutionConfig &migrationConfig = migrationContext.migrationConfig();
    CommandExecution::CommandExecutionContext context(*database, migrationConfig);
    CommandPtrList undoCommands;
    if(migrationName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "migrationName is empty";
        return false;
    }
    CommandPtrList migrationCommands = migrationContext.migrationMap()[migrationName]->commandList();

    MigrationTableServicePtr migrationTableService = migrationContext.baseMigrationTableService();
    if(!migrationTableService) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "migrationTableService is 0";
        return false;
    }

    bool haveTransaction = false;
    if (migrationTableService->canRevertStrucuturalChangesUsingTransactions()) {
        if (!database->transaction()) {
            ::qWarning() << LOG_PREFIX << "starting transaction failed";
            ::qWarning() << LOG_PREFIX << database->lastError();
            return false; // no transaction
        }
        haveTransaction = true;
    } else {
        context.setIsUndoUsed(true);
        undoCommands.reserve(migrationCommands.size());
    }

    ::qDebug() << LOG_PREFIX << "running Migration: " << migrationName;
    CommandServiceRepositoryPtr commandServiceRepository = migrationContext.commandServiceRepository();
    bool isSuccess = m_execution.batch(migrationCommands
                                       , static_cast<CommandExecution::CommandExecutionService::Direction>(direction)
                                       , undoCommands, commandServiceRepository, context);
    if (isSuccess) {
        this->rememberMigration(migrationName, migrationContext, direction);

        if(haveTransaction) {
            return database->commit();
        }
        return true;
    }

    if (haveTransaction) {
        database->rollback();
    } else if (context.isUndoUsed()) {
        context.setIsUndoUsed(false);
        m_execution.batch(undoCommands, CommandExecution::CommandExecutionService::Up
                          , undoCommands, commandServiceRepository, context);
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

    bool bSuccess = true;
    foreach (const QString &migrationName, migrationList) {
        bSuccess &= this->execute(migrationName, context, direction);
        if (!bSuccess) {
            break;
        }
    }
    return bSuccess;
}

bool MigrationExecutionService::isMigrationRemembered(const QString &migrationName
                                                      , const MigrationExecutionContext &context
                                                      , Direction direction) const
{
    CommandExecution::CommandExecutionContext serviceContext(*(context.database()), context.migrationConfig());
    bool isExecuted = context.baseMigrationTableService()->
            wasMigrationExecuted(migrationName, serviceContext);

    switch (direction) {
    case Up:
        return isExecuted;

    case Down:
        return (! isExecuted);

    default:
        return true;
    }
}

bool MigrationExecutionService::rememberMigration(const QString &migrationName
                                                  , const MigrationExecutionContext &context
                                                  , Direction direction) const
{
    CommandExecution::CommandExecutionContext serviceContext(*(context.database()), context.migrationConfig());

    switch (direction) {
    case Up:
        return context.baseMigrationTableService()->addMigration(migrationName, serviceContext);

    case Down:
        return context.baseMigrationTableService()->removeMigration(migrationName, serviceContext);

    default:
        return false;
    }
}

} //namespace MigrationExecution
