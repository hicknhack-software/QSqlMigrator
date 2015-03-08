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
#include "SchemaAlterColumnType.h"

#include "SqlSchemaCommand/AlterColumnType.h"

namespace QSqlMigrator {
namespace SqlMemorySchema {

const SchemaAlterColumnType::CommandType &
SchemaAlterColumnType::commandType() const
{
    return SqlSchemaCommand::AlterColumnType::commandName();
}

bool
SchemaAlterColumnType::execute(const Command &command, Context &context) const
{
    auto alterColumnType = command.staticCast<const SqlSchemaCommand::AlterColumnType>();
    Q_ASSERT(alterColumnType);
    auto tableName = alterColumnType->tableName();
    auto columnName = alterColumnType->columnName();

    if (!context.schema.hasTable(tableName))
        return false; // table not found

    auto originalTable = context.schema.table(tableName);
    if (!originalTable.hasColumn(columnName))
        return false; // column not found

    auto originalColumn = originalTable.column(columnName);
    auto column = SqlSchema::Column(columnName, alterColumnType->newType(), originalColumn.defaultValue(),
                                    originalColumn.attributes());

    auto table = SqlSchema::TableBuilder(originalTable);
    auto success = table.alterColumn(columnName, column);
    if (!success)
        return false; // alter failed

    auto schemaBuilder = SqlSchema::SchemaBuilder(context.schema);
    auto success2 = schemaBuilder.alterTable(tableName, table);
    if (!success2)
        return false; // alter failed
    context.schema = schemaBuilder;
    return true;
}

bool
SchemaAlterColumnType::isValid(const Command &command, const Context &context) const
{
    auto alterColumnType = command.staticCast<const SqlSchemaCommand::AlterColumnType>();
    Q_ASSERT(alterColumnType);
    auto tableName = alterColumnType->tableName();

    auto scope = context.logging->scope([&]() { return QString("Schema%1").arg(alterColumnType->toString()); });
    Q_UNUSED(scope);

    if (!context.schema.hasTable(tableName)) {
        context.logging->error("table does not exist");
        return false;
    }
    const auto table = context.schema.table(alterColumnType->tableName());
    if (!table.hasColumn(alterColumnType->columnName())) {
        context.logging->error("column does not exist");
        return false;
    }
    return true;
}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
