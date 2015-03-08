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
#include "MigrationExecutor.h"

#include <QDebug>

namespace QSqlMigrator {
namespace SqlDatabaseMigrator {
namespace {

using Command = QSharedPointer<const SqlSchemaCommand::Command>;
using Commands = QList<Command>;
using CommandContext = SqlDatabaseSchemaAdapter::CommandExecutorContext;

using MigrationName = MigrationExecutor::MigrationName;
using MigrationTracker = QSharedPointer< SqlMigration::MigrationTracker >;
using CommandExecutors = SqlDatabaseSchemaAdapter::CommandExecutorRepository;

Commands
reverseCommands(const Commands &commands)
{
    Commands reversedCommands;
    reversedCommands.reserve( commands.size() );
    for (const auto &command : commands) {
        if (!command) {
            ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "command is 0!";
            return {};
        }

        auto reversed = command->reverse(command);
        if (reversed.isNull()) {
            return {};
        }
        reversedCommands.prepend(reversed);
    }

    return reversedCommands;
}

bool
isMigrationRemembered(const MigrationName &migrationName,
                      const MigrationTracker &tracker,
                      MigrationExecutor::Direction direction)
{
    bool isExecuted = tracker->wasTracked(migrationName);
    switch (direction) {
    case MigrationExecutor::Apply:
        return isExecuted;

    case MigrationExecutor::Revert:
        return (!isExecuted);

    default:
        return true;
    }
}

bool
rememberMigration(const MigrationName &migrationName,
                  const MigrationTracker &tracker,
                  MigrationExecutor::Direction direction)
{
    switch (direction) {
    case MigrationExecutor::Apply:
        return tracker->track(migrationName);

    case MigrationExecutor::Revert:
        return tracker->forget(migrationName);

    default:
        return false;
    }
}

bool executeCommand(const Command& command,
                    const CommandExecutors& executors,
                    CommandContext& context)
{
    if (!command) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "command is 0!";
        return false;
    }

    auto executor = executors.findExecutor(command->commandName());
    if (!executor) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "service not found!" << command->commandName();
        return false;
    }

    if (executor->isValid(command, context)) {
        return executor->execute(command, context);
    }

    return false;
}

bool executeCommandBatch(const Commands& commands,
                         Commands& undoCommands,
                         const CommandExecutors& executors,
                         CommandContext& context)
{
    for (const auto command : commands) {
        const bool isSuccess = executeCommand(command, executors, context);
        if (!isSuccess) {
            return false; // last command failed
        }

        auto& undo = context.undo();
        if (undo.isUsed()) {
            const auto undoCommand = undo.command();
            undo.resetCommand();
            if (undoCommand) {
                undoCommands.push_front(undoCommand);
            } else {
                ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "command disabled undo" << command->commandName();
                undo.setIsUsed(false);
                undoCommands.clear();
            }
        }
    }
    return true;
}

} // namespace

bool
MigrationExecutor::execute(const MigrationName &migrationName,
                           Direction direction) const
{
    if (migrationName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "migrationName is empty";
        return false; // empty name
    }
    if (!m_setup.migrationTracker) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "migrationTableService is 0";
        return false; // no tracker
    }
    if (isMigrationRemembered(migrationName, m_setup.migrationTracker, direction)) {
        return true; // everything ok
    }

    auto database = m_setup.sqlAdapter.database();
    auto commandContext = CommandContext{ m_setup.sqlAdapter, CommandContext::Config{}, m_setup.logging };

    const auto migration = m_setup.migrations.findMigration(migrationName);
    if (Q_NULLPTR == migration) {
        ::qWarning() << LOG_PREFIX << "migration is missing: " << migrationName;
        return false; // no migration
    }

    auto migrationCommands = migration->commandList();
    if (migrationCommands.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "no commands for migration";
        return false; // no commands
    }

    if (direction == Revert) {
        migrationCommands = reverseCommands(migrationCommands);
        if( migrationCommands.isEmpty() ) {
            ::qWarning() << LOG_PREFIX << "cannot reverse migration commands";
            return false; // no commands
        }
    }

    bool haveTransaction = false;
    Commands undoCommands;
//    if (trackerService->canRevertStructuralChangesUsingTransactions()) {
//        if (!database.transaction()) {
//            ::qWarning() << LOG_PREFIX << "starting transaction failed";
//            ::qWarning() << LOG_PREFIX << database.lastError();
//            return false; // no transaction
//        }
//        haveTransaction = true;
//    } else {
        commandContext.undo().setIsUsed(true);
        undoCommands.reserve(migrationCommands.size());
//    }

    ::qDebug() << LOG_PREFIX << "running Migration: " << migrationName;
    bool isSuccess = executeCommandBatch(migrationCommands, undoCommands, m_setup.commandExecutors, commandContext);
    if (isSuccess) {
        rememberMigration(migrationName, m_setup.migrationTracker, direction);
        if (haveTransaction) {
            return database.commit();
        }
        return true; // success!
    }

    if (haveTransaction) {
        database.rollback();
    }
    else if (commandContext.undo().isUsed()) {
        commandContext.undo().setIsUsed(false);
        executeCommandBatch(undoCommands, undoCommands, m_setup.commandExecutors, commandContext);
    }
    return false; // execution failed
}

bool
MigrationExecutor::executeBatch(const MigrationNames &migrationNames,
                                Direction direction) const
{
    if (migrationNames.empty()) {
        return true; // No migrations present, no need to do anything
    }
    for (const auto &migrationName : migrationNames) {
        bool success = this->execute(migrationName, direction);
        if (!success) {
            return false;
        }
    }
    return true;
}

} // namespace SqlDatabaseMigrator
} // namespace QSqlMigrator
