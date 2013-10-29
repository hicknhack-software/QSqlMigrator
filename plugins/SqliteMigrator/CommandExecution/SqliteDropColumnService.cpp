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
#include "SqliteMigrator/CommandExecution/SqliteDropColumnService.h"

#include "SqliteMigrator/Helper/SqliteDbReaderService.h"

#include "BaseSqlMigrator/CommandExecution/BaseSqlRenameTableService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlCreateTableService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlDropTableService.h"

#include "Commands/CreateTable.h"
#include "Commands/AddColumn.h"
#include "Commands/DropColumn.h"
#include "Commands/DropTable.h"
#include "Commands/RenameTable.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteDropColumnService::SqliteDropColumnService()
{}

bool SqliteDropColumnService::execute(const Commands::ConstCommandPtr &command
                                 , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());

    Structure::Table table = context.helperRepository().dbReaderService().getTableDefinition(dropColumn->tableName(), context.database());
    bool success;
    Structure::Column originalColumn = table.fetchColumnByName(dropColumn->columnName(), success);
    if (!success)
        return success;
    table = Structure::Table::copyWithoutColumn(table, dropColumn->columnName());

    QString tempTableName = QString("%1%2").arg(context.migrationConfig().temporaryTablePrefix
                                                , dropColumn->tableName());

    Commands::CommandPtr renameTable = Commands::CommandPtr(
                new Commands::RenameTable(dropColumn->tableName(), tempTableName));

    BaseSqlRenameTableService renameTableService;
    success = renameTableService.execute(renameTable, context);
    if (!success)
        return false;

    Commands::CommandPtr createTable = Commands::CommandPtr(new Commands::CreateTable(table));
    BaseSqlCreateTableService createTableService;
    success = createTableService.execute(createTable, context);
    if (!success)
        return false;

    QString copyQuery = QString("INSERT INTO %1 SELECT %2 FROM %3").arg(table.name()
                                                                         , table.joinedColumnNames()
                                                                         , tempTableName);
    success = CommandExecution::BaseCommandExecutionService::executeQuery(copyQuery, context);
    if (!success)
        return false;

    Commands::CommandPtr dropTable = Commands::CommandPtr(new Commands::DropTable(tempTableName));
    BaseSqlDropTableService dropTableService;
    success = dropTableService.execute(dropTable, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(Commands::CommandPtr(new Commands::AddColumn(originalColumn
                                                                            , dropColumn->tableName())));
    }

    return success;
}

} // namespace CommandExecution
