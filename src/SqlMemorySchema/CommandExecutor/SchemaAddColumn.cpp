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
#include "SchemaAddColumn.h"

#include "SqlSchemaCommand/AddColumn.h"

namespace QSqlMigrator {
namespace SqlMemorySchema {

const SchemaCommandExecutor::CommandType &
SchemaAddColumn::commandType() const
{
    return SqlSchemaCommand::AddColumn::commandName();
}

bool
SchemaAddColumn::execute(const Command &command, Context &context) const
{
    auto addColumn = command.staticCast<const SqlSchemaCommand::AddColumn>();
    Q_ASSERT(addColumn);
    auto tableName = addColumn->tableName();

    if (!context.schema.hasTable(tableName))
        return false; // table not found

    auto table = SqlSchema::TableBuilder(context.schema.table(tableName));
    table << addColumn->column();

    auto schemaBuilder = SqlSchema::SchemaBuilder(context.schema);
    schemaBuilder.alterTable(tableName, table);
    context.schema = schemaBuilder;
    return true;
}

bool
SchemaAddColumn::isValid(const Command &command, const Context &context) const
{
    auto addColumn = command.staticCast<const SqlSchemaCommand::AddColumn>();
    Q_ASSERT(addColumn);
    auto tableName = addColumn->tableName();

    auto scope = context.logging->scope([&]() { return QString("Schema%1").arg(addColumn->toString()); });
    Q_UNUSED(scope);

    if (!context.schema.hasTable(tableName)) {
        context.logging->error("table does not exist");
        return false;
    }
    const auto table = context.schema.table(tableName);
    if (table.hasColumn(addColumn->column().name())) {
        context.logging->error("column already exists");
        return false;
    }
    return true;
}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
