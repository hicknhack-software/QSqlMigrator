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
#include "SchemaCreateTable.h"

#include "SqlSchemaCommand/CreateTable.h"

namespace QSqlMigrator {
namespace SqlMemorySchema {

const SchemaCommandExecutor::CommandType &
SchemaCreateTable::commandType() const
{
    return SqlSchemaCommand::CreateTable::commandName();
}

bool
SchemaCreateTable::execute(const Command &command, Context &context) const
{
    auto createTable = command.staticCast<const SqlSchemaCommand::CreateTable>();
    Q_ASSERT(createTable);

    auto schemaBuilder = SqlSchema::SchemaBuilder(context.schema);
    schemaBuilder.createTable(createTable->table());
    context.schema = schemaBuilder;
    return true;
}

bool
SchemaCreateTable::isValid(const Command &command, const Context &context) const
{
    auto createTable = command.staticCast<const SqlSchemaCommand::CreateTable>();
    Q_ASSERT(createTable);

    auto scope = context.logging->scope([&]() { return QString("Schema%1").arg(createTable->toString()); });
    Q_UNUSED(scope);

    if (context.schema.hasTable(createTable->table().name())) {
        context.logging->error("table already exists");
        return false;
    }
    return true;
}

} // namespace LocalSchemeCommandService
} // namespace QSqlMigrator
