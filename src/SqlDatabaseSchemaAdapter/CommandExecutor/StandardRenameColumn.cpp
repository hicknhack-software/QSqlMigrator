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
#include "StandardRenameColumn.h"

#include "SqlDatabaseAdapter/Quotation.h"
#include "SqlDatabaseAdapter/SchemaReflection.h"

#include "SqlSchemaCommand/RenameColumn.h"

#include "SqlSchema/Table.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const StandardRenameColumn::CommandType &
StandardRenameColumn::commandType() const
{
    return SqlSchemaCommand::RenameColumn::commandName();
}

bool
StandardRenameColumn::execute(const SqlSchemaCommand::RenameColumn &renameColumn, const Context &context)
{
    const auto quotation = context.sqlAdapter().quotation();
    Q_ASSERT(quotation);
    const auto alterQuery = QString("ALTER TABLE %1 RENAME COLUMN %2 TO %3")
                                .arg(quotation->quoteTableName(renameColumn.tableName()))
                                .arg(quotation->quoteColumnName(renameColumn.oldName()))
                                .arg(quotation->quoteColumnName(renameColumn.newName()));

    return executeQuery(alterQuery, context);
}

bool
StandardRenameColumn::execute(const Command &command, Context &context) const
{
    auto renameColumn = command.staticCast<const SqlSchemaCommand::RenameColumn>();
    Q_ASSERT(renameColumn);

    bool success = execute(*renameColumn, context);
    if (success && context.undo().isUsed()) {
        context.undo().setCommand(command->reverse());
    }
    return success;
}

bool
StandardRenameColumn::isValid(const Command &command, const Context &context) const
{
    auto renameColumn = command.staticCast<const SqlSchemaCommand::RenameColumn>();
    Q_ASSERT(renameColumn);

    auto scope = context.logging()->scope([&]() { return QString("Standard%1").arg(renameColumn->toString()); });
    Q_UNUSED(scope);

    const auto schemaReflection = context.sqlAdapter().schemaReflection();
    Q_ASSERT(schemaReflection);
    const auto table = schemaReflection->getTableDefinition(renameColumn->tableName(), context.database());
    if (!table.isValid()) {
        context.logging()->error("table does not exist");
        return false;
    }
    if (!table.hasColumn(renameColumn->oldName())) {
        context.logging()->error("column does not exist");
        return false;
    }
    if (table.hasColumn(renameColumn->newName())) {
        context.logging()->error("column with new name already exists");
        return false;
    }
    return true;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
