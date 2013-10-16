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
#include "BaseSqlMigrator/CommandExecution/BaseSqlAlterColumnTypeService.h"

#include "Commands/AlterColumnType.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>

namespace CommandExecution {

BaseSqlAlterColumnTypeService::BaseSqlAlterColumnTypeService()
{
}

const QString &BaseSqlAlterColumnTypeService::commandType() const
{
    return Commands::AlterColumnType::typeName();
}

bool BaseSqlAlterColumnTypeService::execute(const Commands::ConstCommandPtr &command
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

    // alter type
    if (modifiedColumn.sqlType() != originalColumn.sqlType()) {
        QString alterQuery = QString("ALTER TABLE %1 ALTER COLUMN %2 SET DATA TYPE %3")
                .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                     , context.helperAggregate().quoteService->quoteColumnName(modifiedColumn.name())
                     , modifiedColumn.sqlType());

        success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

        if (!success)
            return success; // query failed
    }

    // alter nullability
    if (modifiedColumn.isNullable() != originalColumn.isNullable()) {

        QString setOrDrop;
        if (modifiedColumn.isNullable())
            setOrDrop = "SET";
        else
            setOrDrop = "DROP";

        QString alterQuery = QString("ALTER TABLE %1 ALTER COLUMN %2 %3 NOT NULL")
                .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                     , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name())
                     , setOrDrop);

        success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

        if (!success)
            return success; // query failed
    }

    // alter default
    if (modifiedColumn.hasDefaultValue() != originalColumn.hasDefaultValue()) {
        QString setOrDrop;
        if (modifiedColumn.hasDefaultValue())
            setOrDrop = QString("SET DEFAULT %1").arg(modifiedColumn.defaultValue());
        else
            setOrDrop = "DROP DEFAULT";

        QString alterQuery = QString("ALTER TABLE %1 ALTER COLUMN %2 %3")
                .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                     , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name())
                     , setOrDrop);

        success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

        if (!success)
            return success; // query failed
    }

    // alter primary
    if (modifiedColumn.isPrimary() != originalColumn.isPrimary()) {
        if (modifiedColumn.isPrimary()) {
            QString alterQuery = QString("ALTER TABLE %1 ADD PRIMARY KEY (%2)")
                    .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                         , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name()));

            success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

            if (!success)
                return success; // query failed
        } else { // remove primary key constraint
            QString queryString = QString(
                        "SELECT "
                        "    a.constraint_name "
                        "FROM "
                        "    information_schema.table_constraints A, "
                        "    information_schema.constraint_column_usage B "
                        "WHERE "
                        "    a.constraint_name = b.constraint_name AND "
                        "    b.table_name = %1 AND "
                        "    b.column_name = %2 AND "
                        "    constraint_type = %3 " )
                    .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                         , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name())
                         , context.helperAggregate().quoteService->quoteString("PRIMARY KEY"));
            QSqlQuery query = context.database().exec(queryString);
            QSqlError error = query.lastError();
            if (error.isValid()) {
                ::qDebug() << Q_FUNC_INFO << error.text();
            } else {
                // query should return scalar
                QString constraint_name = query.value(0).toString();

                QString alterQuery = QString("ALTER TABLE %1 DROP CONSTRAINT %2")
                        .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                             , constraint_name);

                success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

                if (!success)
                    return success; // query failed
            }
        }
    }

    // alter unique
    if (modifiedColumn.isUnique() != originalColumn.isUnique()) {
        if (modifiedColumn.isPrimary()) {
            QString alterQuery = QString("ALTER TABLE %1 ADD UNIQUE (%2)")
                    .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                         , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name()));

            success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

            if (!success)
                return success; // query failed
        } else { // remove unique constraing
            QString queryString = QString(
                        "SELECT "
                        "    a.constraint_name "
                        "FROM "
                        "    information_schema.table_constraints A, "
                        "    information_schema.constraint_column_usage B "
                        "WHERE "
                        "    a.constraint_name = b.constraint_name AND "
                        "    b.table_name = %1 AND "
                        "    b.column_name = %2 AND "
                        "    constraint_type = %3 ")
                    .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                         , context.helperAggregate().quoteService->quoteColumnName(originalColumn.name())
                         , context.helperAggregate().quoteService->quoteString("UNIQUE"));

            QSqlQuery query = context.database().exec(queryString);
            QSqlError error = query.lastError();
            if (error.isValid()) {
                ::qDebug() << Q_FUNC_INFO << error.text();
            } else {
                // query should return scalar
                QString constraint_name = query.value(0).toString();

                QString alterQuery = QString("ALTER TABLE %1 DROP CONSTRAINT %2")
                        .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                             , constraint_name);

                success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

                if (!success)
                    return success; // query failed
            }
        }
    }

    //alter auto increment not possible here, not in standard SQL

    if (success && context.isUndoUsed()) {
        Commands::CommandPtr undoCommand(new Commands::AlterColumnType(alterColumnType->columnName()
                                                                       , alterColumnType->tableName()
                                                                       , originalColumn.sqlType()
                                                                       , alterColumnType->newType()));
        context.setUndoCommand(undoCommand);
    }

    return success;
}

bool BaseSqlAlterColumnTypeService::isValid(const Commands::ConstCommandPtr &command
                                             , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    //check if table exists
    if (!context.database().tables().contains(alterColumnType->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
