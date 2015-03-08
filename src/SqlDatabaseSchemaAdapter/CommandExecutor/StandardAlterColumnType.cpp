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
#include "StandardAlterColumnType.h"

#include "SqlDatabaseAdapter/SchemaReflection.h"
#include "SqlDatabaseAdapter/ValueTypeMapper.h"
#include "SqlDatabaseAdapter/Quotation.h"

#include "SqlSchemaCommand/AlterColumnType.h"

#include "SqlSchema/Table.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const StandardAlterColumnType::CommandType &
StandardAlterColumnType::commandType() const
{
    return SqlSchemaCommand::AlterColumnType::commandName();
}

bool
StandardAlterColumnType::execute(const SqlSchemaCommand::AlterColumnType &alterColumnType, const Context &context)
{
    const auto quotation = context.sqlAdapter().quotation();
    Q_ASSERT(quotation);
    const auto valueTypeMapper = context.sqlAdapter().valueTypeMapper();
    const QString alterQuery = QString("ALTER TABLE %1 ALTER COLUMN %2 SET DATA TYPE %3")
                                   .arg(quotation->quoteTableName(alterColumnType.tableName()))
                                   .arg(quotation->quoteColumnName(alterColumnType.columnName()))
                                   .arg(valueTypeMapper->map(alterColumnType.newType()));

    return executeQuery(alterQuery, context);
}

bool
StandardAlterColumnType::execute(const Command &command, Context &context) const
{
    auto alterColumnType = command.staticCast<const SqlSchemaCommand::AlterColumnType>();
    Q_ASSERT(alterColumnType);

    const auto schemaReflection = context.sqlAdapter().schemaReflection();
    Q_ASSERT(schemaReflection);
    const auto originalTable = schemaReflection->getTableDefinition(alterColumnType->tableName(), context.database());
    const auto originalColumn = originalTable.column(alterColumnType->columnName());
    if (!originalColumn.isValid()) {
        return false;
    }

    bool success = execute(*alterColumnType, context);

    if (success && context.undo().isUsed()) {
        auto undoCommand = QSharedPointer<SqlSchemaCommand::AlterColumnType>::create(
            alterColumnType->columnName(), alterColumnType->tableName(), originalColumn.type(),
            alterColumnType->newType());
        context.undo().setCommand(undoCommand);
    }

    return success;
}

bool
StandardAlterColumnType::isValid(const Command &command, const Context &context) const
{
    auto alterColumnType = command.staticCast<const SqlSchemaCommand::AlterColumnType>();
    Q_ASSERT(alterColumnType);

    auto scope = context.logging()->scope([&]() { return QString("Standard%1").arg(alterColumnType->toString()); });
    Q_UNUSED(scope);

    const auto schemaReflection = context.sqlAdapter().schemaReflection();
    Q_ASSERT(schemaReflection);
    const auto table = schemaReflection->getTableDefinition(alterColumnType->tableName(), context.database());
    if (!table.isValid()) {
        context.logging()->error("table does not exist");
        return false;
    }
    if (!table.hasColumn(alterColumnType->columnName())) {
        context.logging()->error("column does not exist");
        return false;
    }
    return true;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
