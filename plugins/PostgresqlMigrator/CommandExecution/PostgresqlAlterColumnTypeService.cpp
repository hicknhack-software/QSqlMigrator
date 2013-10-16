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
#include "PostgresqlAlterColumnTypeService.h"

#include "Commands/AlterColumnType.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>

namespace CommandExecution {

PostgresqlAlterColumnTypeService::PostgresqlAlterColumnTypeService()
{
}

bool PostgresqlAlterColumnTypeService::execute(const Commands::ConstCommandPtr &command
                                       , CommandExecution::CommandExecutionContext &context
                                       ) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    Structure::Column originalColumn;
    bool success = true;
    originalColumn = context.helperAggregate()
            .dbReaderService->getTableDefinition(alterColumnType->tableName(), context.database())
            .fetchColumnByName(alterColumnType->columnName(), success);

    if (!success)
        return success; // failed, column doesn't exist

    Structure::Column modifiedColumn(originalColumn.name(), alterColumnType->newType(), originalColumn.attributes());

    // alter {type, nullability, default, primary, unique} is handled by BaseSqlAlterColumnTypeService
    success = this->BaseSqlAlterColumnTypeService::execute(command, context);

    if (!success)
        return success; // execute failed

    QString alterQuery;
    // here we just need to handle auto increment (in PostgreSQL implemented as serial)
    if (modifiedColumn.isAutoIncremented() != originalColumn.isAutoIncremented()) {

        if (modifiedColumn.isAutoIncremented()) { // set auto increment
            alterQuery = QString("CREATE SEQUENCE %1_%2_seq;").arg(alterColumnType->tableName(),
                                                                           alterColumnType->columnName());
            success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

            if (!success)
                return success; // query failed

            // serials also get NOT NULL attribute set, so we need to set that, too
            alterQuery = QString("ALTER TABLE %1 ALTER COLUMN %2 SET NOT NULL")
                    .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                         , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name()));

            success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

            if (!success)
                return success; // query failed

            // auto increment is implemented as default value (call to function), so we need to set it
            alterQuery = QString("ALTER TABLE %1 ALTER COLUMN %2 SET DEFAULT nextval('%1_%2_seq')")
                    .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                         , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name()));

            success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

            if (!success)
                return success; // query failed

            // mark sequence to be "owned by" the column, so that it will be dropped if the column or table is dropped
            alterQuery = QString("ALTER SEQUENCE %1_%2_seq OWNED BY %1.%2").arg(
                        alterColumnType->tableName()
                        , alterColumnType->columnName());

            success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

            if (!success)
                return success; // query failed

        } else { // drop auto increment
            // auto increment is implemented as default value (call to function), so we need to drop it
            alterQuery = QString("ALTER TABLE %1 ALTER COLUMN %2 DROP DEFAULT")
                    .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                         , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name()));

            success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

            if (!success)
                return success; // query failed

            // serials also get NOT NULL attribute set, so we need to remove that, too
            alterQuery = QString("ALTER TABLE %1 ALTER COLUMN %2 DROP NOT NULL")
                    .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                         , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name()));

            success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

            if (!success)
                return success; // query failed
        }
    }

    return success;
}

} // namespace CommandExecution
