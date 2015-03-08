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
#include "SqliteDropColumn.h"

#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardRenameTable.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardCreateTable.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardDropTable.h"

#include "Helper/SqliteSchemaReflection.h"

#include "SqlSchemaCommand/CreateTable.h"
#include "SqlSchemaCommand/AddColumn.h"
#include "SqlSchemaCommand/DropColumn.h"
#include "SqlSchemaCommand/DropTable.h"
#include "SqlSchemaCommand/RenameTable.h"

#include <QDebug>
#include <QStringList>

#include <memory>

namespace SqliteMigrator {
namespace SqlDatabaseSchemaAdapter {

SqliteDropColumn::SqliteDropColumn()
{}

bool SqliteDropColumn::execute(const Command &command,
                               Context &context) const
{
    using namespace QSqlMigrator::SqlDatabaseSchemaAdapter;
    using namespace QSqlMigrator::SqlSchemaCommand;
    using namespace QSqlMigrator::SqlSchema;

    const auto dropColumn(command.staticCast<const DropColumn>());
    const auto table = context.sqlAdapter().schemaReflection()->getTableDefinition(dropColumn->tableName(), context.database());
    auto alteredTable = TableBuilder(dropColumn->tableName());
    const Column* originalColumn = Q_NULLPTR;

    foreach (const Column &column, table.columns()) {
        if (column.name() == dropColumn->columnName()) {
            originalColumn = &column;
        } else {
            alteredTable << column;
        }
    }
    if (!originalColumn) {
        ::qWarning() << "Column not found" << dropColumn->tableName() << dropColumn->columnName();
        return false;
    }

    const auto tempTableName = QString("%1%2").arg(context.config().temporaryTablePrefix, dropColumn->tableName());

    bool success = StandardRenameTable::execute(RenameTable(dropColumn->tableName(), tempTableName), context);
    if (!success)
        return false;

    success = StandardCreateTable::execute(CreateTable(alteredTable), context);
    if (!success)
        return false;

    const QString copyQuery =
            QString("INSERT INTO %1 SELECT %2 FROM %3")
            .arg(table.name())
            .arg(table.columnNames().join(", "))
            .arg(tempTableName);
    success = CommandExecutor::executeQuery(copyQuery, context);
    if (!success)
        return false;

    success = StandardDropTable::execute(DropTable(tempTableName), context);

    if (success && context.undo().isUsed()) {
        context.undo().setCommand(
                    QSharedPointer<AddColumn>::create(dropColumn->tableName(), *originalColumn)
                    );
    }
    return success;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace SqliteMigrator
