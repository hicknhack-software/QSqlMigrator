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
#include "SchemaDropIndex.h"

#include "SqlSchemaCommand/DropIndex.h"

namespace QSqlMigrator {
namespace SqlMemorySchema {

const SchemaCommandExecutor::CommandType &
SchemaDropIndex::commandType() const
{
    return SqlSchemaCommand::DropIndex::commandName();
}

bool
SchemaDropIndex::execute(const Command &command, Context &context) const
{
    const auto dropIndex = command.staticCast<const SqlSchemaCommand::DropIndex>();
    Q_ASSERT(dropIndex);

    auto schemaBuilder = SqlSchema::SchemaBuilder(context.schema);
    bool success = schemaBuilder.dropIndex(dropIndex->name());
    if (!success)
        return false; // drop failed

    context.schema = schemaBuilder;
    return true;
}

bool
SchemaDropIndex::isValid(const Command &command, const Context &context) const
{
    const auto dropIndex = command.staticCast<const SqlSchemaCommand::DropIndex>();
    Q_ASSERT(dropIndex);

    auto scope = context.logging->scope([&]() { return QString("Schema%1").arg(dropIndex->toString()); });
    Q_UNUSED(scope);

    const auto index = context.schema.index(dropIndex->name());
    if (!index.isValid()) {
        context.logging->error("index does not exist");
        return false;
    }
    return true;
}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
