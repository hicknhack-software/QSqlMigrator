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
#include "SqliteMigrator/CommandExecution/SqliteAlterColumnTypeService.h"

#include "SqliteMigrator/CommandExecution/SqliteAlterColumnService.h"
#include "SqliteMigrator/Helper/SqliteDbReaderService.h"

#include "Commands/AlterColumnType.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteAlterColumnTypeService::SqliteAlterColumnTypeService()
{
}

bool SqliteAlterColumnTypeService::execute(const Commands::ConstCommandPtr &command
                                      , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    Structure::Table origTable = context.helperAggregate().dbReaderService->getTableDefinition(alterColumnType->tableName(), context.database());
    Structure::Table newTable = Structure::Table(alterColumnType->tableName());
    Structure::Column originalColumn("", "");

    foreach (Structure::Column column, origTable.columns()) {
        if (column.name() == alterColumnType->columnName()) {
            originalColumn = column;
            Structure::Column newColumn = Structure::Column(column.name(), alterColumnType->newType(), column.attributes());
            newTable.add(newColumn);
        } else {
            newTable.add(column);
        }
    }

    SqliteAlterColumnService alterColumnService;
    bool success = alterColumnService.execute(origTable, newTable, context);

    //TODO: add test for undo logic of SqliteMigrator command execution services
    if (success && context.isUndoUsed()) {
        Commands::CommandPtr undoCommand(new Commands::AlterColumnType(alterColumnType->columnName()
                                                                       , alterColumnType->tableName()
                                                                       , originalColumn.sqlType()
                                                                       , alterColumnType->newType()));
        context.setUndoCommand(undoCommand);
    }

    return success;
}

} // namespace CommandExecution
