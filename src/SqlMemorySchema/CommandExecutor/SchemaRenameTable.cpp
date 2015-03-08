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
#include "SchemaRenameTable.h"

#include "SqlSchemaCommand/RenameTable.h"

namespace QSqlMigrator {
namespace SqlMemorySchema {

const SchemaCommandExecutor::CommandType &
SchemaRenameTable::commandType() const
{
    return SqlSchemaCommand::RenameTable::commandName();
}

bool
SchemaRenameTable::execute(const Command &command, Context &context) const
{
    auto renameTable = command.staticCast<const SqlSchemaCommand::RenameTable>();
    Q_ASSERT(renameTable);
    const auto tableName = renameTable->oldName();

    if (!context.schema.hasTable(tableName))
        return false; // table not found

    const auto originalTable = context.schema.table(tableName);

    auto table = SqlSchema::TableBuilder(originalTable);
    table.setName(renameTable->newName());

    auto schemaBuilder = SqlSchema::SchemaBuilder(context.schema);
    const auto success = schemaBuilder.alterTable(tableName, table);
    if (!success)
        return false; // alter failed

    context.schema = schemaBuilder;
    return true;
}

bool
SchemaRenameTable::isValid(const Command &command, const Context &context) const
{
    auto renameTable = command.staticCast<const SqlSchemaCommand::RenameTable>();
    Q_ASSERT(renameTable);

    auto scope = context.logging->scope([&]() { return QString("Schema%1").arg(renameTable->toString()); });
    Q_UNUSED(scope);

    if (!context.schema.hasTable(renameTable->oldName())) {
        context.logging->error("table does not exist");
        return false;
    }
    if (context.schema.hasTable(renameTable->newName())) {
        context.logging->error("table with new name already exists");
        return false;
    }
    return true;
}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
