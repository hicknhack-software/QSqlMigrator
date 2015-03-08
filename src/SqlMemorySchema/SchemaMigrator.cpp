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
#include "SchemaMigrator.h"

#include "SqlMemorySchema/CommandExecutor/SchemaAddColumn.h"
#include "SqlMemorySchema/CommandExecutor/SchemaAlterColumnType.h"
#include "SqlMemorySchema/CommandExecutor/SchemaCreateIndex.h"
#include "SqlMemorySchema/CommandExecutor/SchemaCreateTable.h"
#include "SqlMemorySchema/CommandExecutor/SchemaDropColumn.h"
#include "SqlMemorySchema/CommandExecutor/SchemaDropIndex.h"
#include "SqlMemorySchema/CommandExecutor/SchemaDropTable.h"
#include "SqlMemorySchema/CommandExecutor/SchemaRenameColumn.h"
#include "SqlMemorySchema/CommandExecutor/SchemaRenameTable.h"

#include <QDebug>

namespace QSqlMigrator {
namespace SqlMemorySchema {
namespace {

using Command = QSharedPointer<const SqlSchemaCommand::Command>;
using Commands = QList<Command>;
using CommandContext = SchemaCommandExecutor::Context;
using CommandExecutors = SchemaCommandExecutorRepository;

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
                         const CommandExecutors& executors,
                         CommandContext& context)
{
    for (const auto command : commands) {
        const bool isSuccess = executeCommand(command, executors, context);
        if (!isSuccess) {
            return false; // last command failed
        }
    }
    return true;
}

}

SchemaCommandExecutorRepository
createCommandRepository()
{
    auto commandRepository = SchemaCommandExecutorRepository{};
    commandRepository.add(QSharedPointer<SchemaAddColumn>::create());
    commandRepository.add(QSharedPointer<SchemaAlterColumnType>::create());
    commandRepository.add(QSharedPointer<SchemaCreateIndex>::create());
    commandRepository.add(QSharedPointer<SchemaCreateTable>::create());
    commandRepository.add(QSharedPointer<SchemaDropColumn>::create());
    commandRepository.add(QSharedPointer<SchemaDropIndex>::create());
    commandRepository.add(QSharedPointer<SchemaDropTable>::create());
    commandRepository.add(QSharedPointer<SchemaRenameColumn>::create());
    commandRepository.add(QSharedPointer<SchemaRenameTable>::create());
    return commandRepository;
}

bool
SchemaMigrator::applyMigration(const MigrationName &migrationName, Context &context) const
{
    if (migrationName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "migrationName is empty";
        return false; // no migtration given
    }

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

    ::qDebug() << LOG_PREFIX << "running Migration on local scheme: " << migrationName;

    auto commandContext = CommandContext { context.schema, context.logging };
    bool isSuccess = executeCommandBatch(migrationCommands, m_setup.commandExecutors, commandContext);

    return isSuccess;
}

bool
SchemaMigrator::applyAll(const MigrationNames &migrationNames, Context &context) const
{
    if (migrationNames.empty()) {
        return true; // No migrations present, no need to do anything
    }
    for (const auto &migrationName : migrationNames) {
        bool success = this->applyMigration(migrationName, context);
        if (!success) {
            return false;
        }
    }
    return true;
}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
