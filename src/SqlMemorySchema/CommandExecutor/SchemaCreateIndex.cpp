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
#include "SchemaCreateIndex.h"

#include "SqlSchemaCommand/CreateIndex.h"

#include <QDebug>

namespace QSqlMigrator {
namespace SqlMemorySchema {

const SchemaCommandExecutor::CommandType &
SchemaCreateIndex::commandType() const
{
    return SqlSchemaCommand::CreateIndex::commandName();
}

bool
SchemaCreateIndex::execute(const Command &command, Context &context) const
{
    auto createIndex = command.staticCast<const SqlSchemaCommand::CreateIndex>();
    Q_ASSERT(createIndex);

    auto schemaBuilder = SqlSchema::SchemaBuilder(context.schema);
    schemaBuilder.createIndex(createIndex->index());
    context.schema = schemaBuilder;
    return true;
}

bool
SchemaCreateIndex::isValid(const Command &command, const Context &context) const
{
    auto createIndex = command.staticCast<const SqlSchemaCommand::CreateIndex>();
    Q_ASSERT(createIndex);

    auto scope = context.logging->scope([&]() { return QString("Schema%1").arg(createIndex->toString()); });
    Q_UNUSED(scope);

    if (context.schema.hasIndex(createIndex->index().name())) {
        context.logging->error("index already exists");
        return false;
    }
    if (context.schema.hasTable(createIndex->index().tableName())) {
        context.logging->error("table does not exist");
        return false;
    }

    return true;
}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
