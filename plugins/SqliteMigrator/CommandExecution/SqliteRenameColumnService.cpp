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
#include "SqliteMigrator/CommandExecution/SqliteRenameColumnService.h"

#include "SqliteMigrator/CommandExecution/SqliteAlterColumnService.h"

#include "Commands/RenameColumn.h"

#include <QDebug>
#include <QStringList>

using namespace Structure;

namespace CommandExecution {

SqliteRenameColumnService::SqliteRenameColumnService()
{
}

bool SqliteRenameColumnService::execute(const Commands::ConstCommandPtr &command
                                   , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());

    Table origTable = context.helperAggregate().dbReaderService->getTableDefinition(renameColumn->tableName(), context.database());
    Table table = Table(renameColumn->tableName());
    foreach (Column column, origTable.columns()) {
        if (column.name() == renameColumn->name()) {
            Column newColumn = Column(renameColumn->newName(), column.sqlTypeString(), column.attributes());
            table.add(newColumn);
        } else {
            table.add(column);
        }
    }

    SqliteAlterColumnService alterColumnService;

    bool success = alterColumnService.execute(origTable, table, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(renameColumn->reverse());
    }

    return success;
}

} // namespace CommandExecution
