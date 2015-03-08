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
#include "StandardDropColumn.h"

#include "SqlDatabaseAdapter/SchemaReflection.h"
#include "SqlDatabaseAdapter/Quotation.h"

#include "SqlSchemaCommand/DropColumn.h"
#include "SqlSchemaCommand/AddColumn.h"

#include "SqlSchema/Table.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const StandardDropColumn::CommandType &
StandardDropColumn::commandType() const
{
    return SqlSchemaCommand::DropColumn::commandName();
}

bool
StandardDropColumn::execute(const SqlSchemaCommand::DropColumn &dropColumn, const Context &context)
{
    const auto quotation = context.sqlAdapter().quotation();
    Q_ASSERT(quotation);
    const auto alterQuery = QString("ALTER TABLE %1 DROP COLUMN %2")
                                .arg(quotation->quoteTableName(dropColumn.tableName()))
                                .arg(quotation->quoteColumnName(dropColumn.columnName()));

    return executeQuery(alterQuery, context);
}

bool
StandardDropColumn::execute(const Command &command, Context &context) const
{
    const auto dropColumn = command.staticCast<const SqlSchemaCommand::DropColumn>();
    Q_ASSERT(dropColumn);

    const auto schemaReflection = context.sqlAdapter().schemaReflection();
    Q_ASSERT(schemaReflection);
    const auto originalTable = schemaReflection->getTableDefinition(dropColumn->tableName(), context.database());
    const auto originalColumn = originalTable.column(dropColumn->columnName());

    bool success = execute(*dropColumn, context);
    if (success && context.undo().isUsed()) {
        auto undoCommand = QSharedPointer<SqlSchemaCommand::AddColumn>::create(dropColumn->tableName(), originalColumn);
        context.undo().setCommand(undoCommand);
        return true;
    }
    return success;
}

bool
StandardDropColumn::isValid(const Command &command, const Context &context) const
{
    const auto dropColumn = command.staticCast<const SqlSchemaCommand::DropColumn>();
    Q_ASSERT(dropColumn);

    auto scope = context.logging()->scope([&]() { return QString("Standard%1").arg(dropColumn->toString()); });
    Q_UNUSED(scope);

    const auto schemaReflection = context.sqlAdapter().schemaReflection();
    Q_ASSERT(schemaReflection);
    const auto table = schemaReflection->getTableDefinition(dropColumn->tableName(), context.database());
    if (!table.isValid()) {
        context.logging()->error("table does not exist");
        return false;
    }
    if (!table.hasColumn(dropColumn->columnName())) {
        context.logging()->error("column does not exist");
        return false;
    }
    return true;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
