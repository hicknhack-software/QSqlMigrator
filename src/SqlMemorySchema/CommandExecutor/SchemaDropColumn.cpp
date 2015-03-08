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
#include "SchemaDropColumn.h"

#include "SqlSchemaCommand/DropColumn.h"

namespace QSqlMigrator {
namespace SqlMemorySchema {

const SchemaCommandExecutor::CommandType &
SchemaDropColumn::commandType() const
{
    return SqlSchemaCommand::DropColumn::commandName();
}

bool
SchemaDropColumn::execute(const Command &command, Context &context) const
{
    const auto dropColumn = command.staticCast<const SqlSchemaCommand::DropColumn>();
    Q_ASSERT(dropColumn);
    const auto tableName = dropColumn->tableName();

    if (!context.schema.hasTable(tableName))
        return false; // table not found

    const auto originalTable = context.schema.table(tableName);
    auto table = SqlSchema::TableBuilder(originalTable);
    const auto success = table.dropColumn(dropColumn->columnName());
    if (!success)
        return false; // drop failed

    auto schemaBuilder = SqlSchema::SchemaBuilder(context.schema);
    const auto success2 = schemaBuilder.alterTable(tableName, table);
    if (!success2)
        return false; // alter failed
    context.schema = schemaBuilder;
    return true;
}

bool
SchemaDropColumn::isValid(const Command &command, const Context &context) const
{
    const auto dropColumn = command.staticCast<const SqlSchemaCommand::DropColumn>();
    Q_ASSERT(dropColumn);
    const auto tableName = dropColumn->tableName();

    auto scope = context.logging->scope([&]() { return QString("Schema%1").arg(dropColumn->toString()); });
    Q_UNUSED(scope);

    if (!context.schema.hasTable(tableName)) {
        context.logging->error("table does not exist");
        return false;
    }
    const auto table = context.schema.table(tableName);
    if (!table.hasColumn(dropColumn->columnName())) {
        context.logging->error("column does not exist");
        return false;
    }
    return true;
}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
