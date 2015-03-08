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
#include "StandardRenameTable.h"

#include "SqlDatabaseAdapter/Quotation.h"

#include "SqlSchemaCommand/RenameTable.h"

#include <QDebug>
#include <QStringList>

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const StandardRenameTable::CommandType &
StandardRenameTable::commandType() const
{
    return SqlSchemaCommand::RenameTable::commandName();
}

bool
StandardRenameTable::execute(const SqlSchemaCommand::RenameTable &renameTable, const Context &context)
{
    const auto quotation = context.sqlAdapter().quotation();
    Q_ASSERT(quotation);
    const auto alterQuery = QString("ALTER TABLE %1 RENAME TO %2")
                                .arg(quotation->quoteTableName(renameTable.oldName()))
                                .arg(quotation->quoteTableName(renameTable.newName()));

    return executeQuery(alterQuery, context);
}

bool
StandardRenameTable::execute(const Command &command, Context &context) const
{
    auto renameTable = command.staticCast<const SqlSchemaCommand::RenameTable>();
    Q_ASSERT(renameTable);

    bool success = execute(*renameTable, context);
    if (success && context.undo().isUsed()) {
        context.undo().setCommand(command->reverse());
    }
    return success;
}

bool
StandardRenameTable::isValid(const Command &command, const Context &context) const
{
    auto renameTable = command.staticCast<const SqlSchemaCommand::RenameTable>();
    Q_ASSERT(renameTable);

    auto scope = context.logging()->scope([&]() { return QString("Standard%1").arg(renameTable->toString()); });
    Q_UNUSED(scope);

    const auto tables = context.database().tables();
    if (!tables.contains(renameTable->oldName())) {
        context.logging()->error("table does not exist");
        return false;
    }
    if (tables.contains(renameTable->newName())) {
        context.logging()->error("table with new name already exists");
        return false;
    }
    return true;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
