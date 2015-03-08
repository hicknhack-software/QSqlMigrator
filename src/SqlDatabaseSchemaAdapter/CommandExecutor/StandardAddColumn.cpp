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
#include "StandardAddColumn.h"

#include "SqlDatabaseAdapter/ColumnMapper.h"
#include "SqlDatabaseAdapter/Quotation.h"
#include "SqlDatabaseAdapter/SchemaReflection.h"

#include "SqlSchemaCommand/AddColumn.h"
#include "SqlSchemaCommand/DropColumn.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const StandardAddColumn::CommandType &
StandardAddColumn::commandType() const
{
    return SqlSchemaCommand::AddColumn::commandName();
}

bool
StandardAddColumn::execute(const SqlSchemaCommand::AddColumn &addColumn, const Context &context)
{
    const auto columnMapper = context.sqlAdapter().columnMapper();
    Q_ASSERT(columnMapper);
    const auto quotation = context.sqlAdapter().quotation();
    Q_ASSERT(quotation);
    const auto columnDefinition = columnMapper->generateColumnDefinitionSql(addColumn.column());
    const auto alterQuery = QString("ALTER TABLE %1 ADD COLUMN %2")
                                .arg(quotation->quoteTableName(addColumn.tableName()))
                                .arg(columnDefinition);

    return executeQuery(alterQuery, context);
}

bool
StandardAddColumn::execute(const Command &command, Context &context) const
{
    auto addColumn = command.staticCast<const SqlSchemaCommand::AddColumn>();
    Q_ASSERT(addColumn);

    bool success = execute(*addColumn, context);

    if (success && context.undo().isUsed())
        context.undo().setCommand(command->reverse());

    return success;
}

bool
StandardAddColumn::isValid(const Command &command, const Context &context) const
{
    auto addColumn = command.staticCast<const SqlSchemaCommand::AddColumn>();
    Q_ASSERT(addColumn);

    auto scope = context.logging()->scope([&]() { return QString("Standard%1").arg(addColumn->toString()); });
    Q_UNUSED(scope);

    const auto schemaReflection = context.sqlAdapter().schemaReflection();
    Q_ASSERT(schemaReflection);
    const auto table = schemaReflection->getTableDefinition(addColumn->tableName(), context.database());

    if (!table.isValid()) {
        context.logging()->error("table does not exist");
        return false;
    }
    if (table.hasColumn(addColumn->column().name())) {
        context.logging()->error("column already exists");
        return false;
    }
    return true;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
