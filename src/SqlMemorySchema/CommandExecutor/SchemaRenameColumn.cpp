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
#include "SchemaRenameColumn.h"

#include "SqlSchemaCommand/RenameColumn.h"

namespace QSqlMigrator {
namespace SqlMemorySchema {

const SchemaCommandExecutor::CommandType &
SchemaRenameColumn::commandType() const
{
    return SqlSchemaCommand::RenameColumn::commandName();
}

bool
SchemaRenameColumn::execute(const Command &command, Context &context) const
{
    auto renameColumn = command.staticCast<const SqlSchemaCommand::RenameColumn>();
    Q_ASSERT(renameColumn);
    const auto tableName = renameColumn->tableName();
    const auto columnName = renameColumn->oldName();

    if (!context.schema.hasTable(tableName))
        return false; // table not found

    const auto originalTable = context.schema.table(tableName);
    if (!originalTable.hasColumn(columnName))
        return false; // column not found

    const auto originalColumn = originalTable.column(columnName);
    const auto column = SqlSchema::Column(renameColumn->newName(), originalColumn.type(), originalColumn.defaultValue(),
                                          originalColumn.attributes());

    auto table = SqlSchema::TableBuilder(originalTable);
    const auto success = table.alterColumn(columnName, column);
    if (!success)
        return false; // alter failed

    auto schemaBuilder = SqlSchema::SchemaBuilder(context.schema);
    const auto success2 = schemaBuilder.alterTable(tableName, table);
    if (!success2)
        return false; // alter failed

    context.schema = schemaBuilder;
    return true;
}

bool
SchemaRenameColumn::isValid(const Command &command, const Context &context) const
{
    auto renameColumn = command.staticCast<const SqlSchemaCommand::RenameColumn>();
    Q_ASSERT(renameColumn);
    const auto tableName = renameColumn->tableName();

    auto scope = context.logging->scope([&]() { return QString("Schema%1").arg(renameColumn->toString()); });
    Q_UNUSED(scope);

    if (!context.schema.hasTable(tableName)) {
        context.logging->error("table does not exist");
        return false;
    }
    const auto table = context.schema.table(tableName);
    if (!table.hasColumn(renameColumn->oldName())) {
        context.logging->error("column does not exist");
        return false;
    }
    if (table.hasColumn(renameColumn->newName())) {
        context.logging->error("new column already exists");
        return false;
    }
    return true;
}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
