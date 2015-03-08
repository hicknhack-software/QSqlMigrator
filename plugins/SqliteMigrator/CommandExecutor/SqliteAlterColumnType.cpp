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
#include "SqliteAlterColumnType.h"

#include "SqliteMigrator/CommandExecutor/SqliteAlterColumn.h"
#include "SqliteMigrator/Helper/SqliteSchemaReflection.h"

#include "SqlDatabaseAdapter/ValueTypeMapper.h"
#include "SqlDatabaseAdapter/StandardColumnMapper.h"

#include "SqlSchemaCommand/AlterColumnType.h"

#include "SqlSchema/Table.h"

#include <QDebug>
#include <QStringList>

namespace SqliteMigrator {
namespace SqlDatabaseSchemaAdapter {

SqliteAlterColumnType::SqliteAlterColumnType()
{
}

bool
SqliteAlterColumnType::execute(const Command &command, Context &context) const
{
    using namespace QSqlMigrator::SqlSchemaCommand;
    using namespace QSqlMigrator::SqlSchema;

    const auto alterColumnType(command.staticCast<const AlterColumnType>());
    const auto origTable = context.sqlAdapter().schemaReflection()->getTableDefinition(
        alterColumnType->tableName(), context.database());
    auto alteredTable = TableBuilder(alterColumnType->tableName());

    const Column *originalColumn = Q_NULLPTR;
    foreach (const Column &column, origTable.columns()) {
        if (column.name() == alterColumnType->columnName()) {
            originalColumn = &column;
            alteredTable << Column(column.name(), alterColumnType->newType(), column.attributes());
        }
        else {
            alteredTable << column;
        }
    }
    if (!originalColumn) {
        ::qWarning() << "column was not found!" << alterColumnType->tableName()
                     << alterColumnType->columnName();
        return false;
    }

    bool success = SqliteAlterColumn::execute(origTable, alteredTable, context);
    if (success && context.undo().isUsed()) {
        auto undoCommand = QSharedPointer<AlterColumnType>::create(
            alterColumnType->columnName(), alterColumnType->tableName(), originalColumn->type(),
            alterColumnType->newType());
        context.undo().setCommand(undoCommand);
    }
    return success;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace SqliteMigrator
