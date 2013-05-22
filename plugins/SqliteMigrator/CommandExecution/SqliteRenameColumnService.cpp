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
#include "SqliteMigrator/Helper/SqliteDbReader.h"

#include "Commands/RenameColumn.h"

#include <QDebug>
#include <QStringList>

using namespace Structure;

namespace CommandExecution {

SqliteRenameColumnService::SqliteRenameColumnService()
{
}

const QString &SqliteRenameColumnService::commandType() const
{
    return Commands::RenameColumn::typeName();
}

bool SqliteRenameColumnService::up(const Commands::ConstCommandPtr &command
                                   , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());

    Helper::SqliteDbReader dbReader;
    Table origTable = dbReader.getTableDefinition(renameColumn->tableName(), context);
    Table table = Table(renameColumn->tableName());
    foreach (Column column, origTable.columns()) {
        if (column.name() == renameColumn->name()) {
            Column newColumn = Column(renameColumn->newName(), column.sqlType(), column.attributes());
            table.add(newColumn);
        } else {
            table.add(column);
        }
    }

    SqliteAlterColumnService alterColumnService;
    return alterColumnService.run(origTable, table, context);
}

bool SqliteRenameColumnService::isUpValid(const Commands::ConstCommandPtr &command
                                          , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());

    //check if table exists
    if (!context.database().tables().contains(renameColumn->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }
    return true;
}

bool SqliteRenameColumnService::down(const Commands::ConstCommandPtr &command
                                     , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());
    return this->up(Commands::CommandPtr(new Commands::RenameColumn(renameColumn->newName()
                                                                    , renameColumn->name()
                                                                    , renameColumn->tableName()))
                    , context);
}

bool SqliteRenameColumnService::isDownValid(const Commands::ConstCommandPtr &command
                                            , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());

    //check if table exists
    if (!context.database().tables().contains(renameColumn->newName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
