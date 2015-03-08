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
#include "SqliteRenameColumn.h"

#include "SqliteMigrator/CommandExecutor/SqliteAlterColumn.h"

#include "Helper/SqliteSchemaReflection.h"

#include "SqlSchemaCommand/RenameColumn.h"

#include "SqlSchema/Table.h"

#include <QDebug>
#include <QStringList>

namespace SqliteMigrator {
namespace SqlDatabaseSchemaAdapter {

SqliteRenameColumn::SqliteRenameColumn()
{
}

bool SqliteRenameColumn::execute(const Command &command,
                                 Context &context) const
{
    using namespace QSqlMigrator::SqlSchemaCommand;
    using namespace QSqlMigrator::SqlSchema;

    auto renameColumn(command.staticCast<const RenameColumn>());

    const auto table = context.sqlAdapter().schemaReflection()->getTableDefinition(renameColumn->tableName(), context.database());
    auto alteredTable = TableBuilder(renameColumn->tableName());

    bool found = false;
    foreach (const Column &column, table.columns()) {
        if (column.name() == renameColumn->oldName()) {
            alteredTable << Column(renameColumn->newName(), column.type(), column.attributes());
            found = true;
        } else {
            alteredTable << column;
        }
    }
    if (!found) {
        ::qWarning() << "Column not found" << renameColumn->oldName();
        return false;
    }

    bool success = SqliteAlterColumn::execute(table, alteredTable, context);
    if (success && context.undo().isUsed()) {
        context.undo().setCommand(command->reverse());
    }
    return success;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace SqliteMigrator
