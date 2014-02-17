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
#include "SqliteMigrator/Helper/SqliteSqlStructureService.h"

#include "Helper/TypeMapperService.h"
#include "Helper/SqlStructureService.h"

#include "Commands/AlterColumnType.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteAlterColumnTypeService::SqliteAlterColumnTypeService()
{
}

bool SqliteAlterColumnTypeService::execute(const Commands::ConstCommandPtr &command,
                                           CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    const Structure::Table origTable( context.helperRepository().sqlStructureService().getTableDefinition(alterColumnType->tableName(), context.database()) );
    Structure::Table::Builder alteredTable(alterColumnType->tableName());
    const Structure::Column* originalColumn = Q_NULLPTR;

    QString newType = context.helperRepository().typeMapperService().map(alterColumnType->newType());

    foreach (const Structure::Column &column, origTable.columns()) {
        if (column.name() == alterColumnType->columnName()) {
            originalColumn = &column;
            alteredTable << Structure::Column(column.name(), newType, column.attributes());
        } else {
            alteredTable << column;
        }
    }
    if (!originalColumn) {
        ::qWarning() << "column was not found!" << alterColumnType->tableName() << alterColumnType->columnName();
        return false;
    }

    bool success = SqliteAlterColumnService::execute(origTable, alteredTable, context);

    //TODO: add test for undo logic of SqliteMigrator command execution services
    if (success && context.isUndoUsed()) {
        Commands::CommandPtr undoCommand(new Commands::AlterColumnType(alterColumnType->columnName(),
                                                                       alterColumnType->tableName(),
                                                                       originalColumn->type(),
                                                                       newType));
        context.setUndoCommand(undoCommand);
    }

    return success;
}

} // namespace CommandExecution
