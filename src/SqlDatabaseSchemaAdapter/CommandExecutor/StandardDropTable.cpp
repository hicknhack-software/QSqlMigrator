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
#include "StandardDropTable.h"

#include "SqlDatabaseAdapter/SchemaReflection.h"
#include "SqlDatabaseAdapter/Quotation.h"

#include "SqlSchemaCommand/DropTable.h"
#include "SqlSchemaCommand/CreateTable.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const StandardDropTable::CommandType &
StandardDropTable::commandType() const
{
    return SqlSchemaCommand::DropTable::commandName();
}

bool
StandardDropTable::execute(const SqlSchemaCommand::DropTable &dropTable, const Context &context)
{
    const auto quotation = context.sqlAdapter().quotation();
    Q_ASSERT(quotation);
    const auto dropQuery = QString("DROP TABLE %1").arg(quotation->quoteTableName(dropTable.tableName()));

    return executeQuery(dropQuery, context);
}

bool
StandardDropTable::execute(const Command &command, Context &context) const
{
    auto dropTable = command.staticCast<const SqlSchemaCommand::DropTable>();
    Q_ASSERT(dropTable);

    const auto schemaReflection = context.sqlAdapter().schemaReflection();
    const auto originalTable = schemaReflection->getTableDefinition(dropTable->tableName(), context.database());

    bool success = execute(*dropTable, context);
    if (success && context.undo().isUsed()) {
        context.undo().setCommand(QSharedPointer<SqlSchemaCommand::CreateTable>::create(originalTable));
    }
    return success;
}

bool
StandardDropTable::isValid(const Command &command, const Context &context) const
{
    auto dropTable = command.staticCast<const SqlSchemaCommand::DropTable>();
    Q_ASSERT(dropTable);

    auto scope = context.logging()->scope([&]() { return QString("Standard%1").arg(dropTable->toString()); });
    Q_UNUSED(scope);

    if (!context.database().tables().contains(dropTable->tableName())) {
        context.logging()->error("table does not exist");
        return false;
    }
    return true;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
