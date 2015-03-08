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
#include "SchemaDropTable.h"

#include "SqlSchemaCommand/DropTable.h"

namespace QSqlMigrator {
namespace SqlMemorySchema {

const QString &SchemaDropTable::commandType() const
{
    return SqlSchemaCommand::DropTable::commandName();
}

bool
SchemaDropTable::execute(const Command &command, Context &context) const
{
    auto dropTable = command.staticCast<const SqlSchemaCommand::DropTable>();
    Q_ASSERT(dropTable);

    auto schemaBuilder = SqlSchema::SchemaBuilder(context.schema);
    const auto success = schemaBuilder.dropTable(dropTable->tableName());
    if (!success)
        return false; // drop failed

    context.schema = schemaBuilder;
    return true;
}

bool
SchemaDropTable::isValid(const Command &command, const Context &context) const
{
    auto dropTable = command.staticCast<const SqlSchemaCommand::DropTable>();
    Q_ASSERT(dropTable);

    auto scope = context.logging->scope([&]() { return QString("Schema%1").arg(dropTable->toString()); });
    Q_UNUSED(scope);

    if (!context.schema.hasTable(dropTable->tableName())) {
        context.logging->error("table does not exist");
        return false;
    }
    return true;
}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
