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

#include "SqliteMigrator/CommandExecution/SqliteAddColumnService.h"
#include "SqliteMigrator/CommandExecution/SqliteCreateTableService.h"
#include "SqliteMigrator/CommandExecution/SqliteDropTableService.h"
#include "SqliteMigrator/CommandExecution/SqliteRenameTableService.h"
#include "SqliteMigrator/Helper/SqliteDbReader.h"

#include "Commands/AddColumn.h"
#include "Commands/CreateTable.h"
#include "Commands/DropColumn.h"
#include "Commands/DropTable.h"
#include "Commands/RenameTable.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteDropColumnService::SqliteDropColumnService()
{
}

const QString &SqliteDropColumnService::commandType() const
{
    return Commands::DropColumn::typeName();
}

bool SqliteDropColumnService::up(const Commands::ConstCommandPtr &command
                                 , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());

    Helper::SqliteDbReader dbReader;
    Structure::Table table = dbReader.getTableDefinition(dropColumn->tableName(), context);
    table = Structure::Table::copyWithoutColumn(table, dropColumn->columnName());

    QString tempTableName = QString("%1%2").arg(context.migrationConfig().temporaryTablePrefix
                                                , dropColumn->tableName());

    bool bSuccess;
    Commands::CommandPtr renameTable = Commands::CommandPtr(
                new Commands::RenameTable(dropColumn->tableName(), tempTableName));
    SqliteRenameTableService renameTableService;
    bSuccess = renameTableService.up(renameTable, context);
    if (!bSuccess) {return false;}

    Commands::CommandPtr createTable = Commands::CommandPtr(new Commands::CreateTable(table));
    SqliteCreateTableService createTableService;
    bSuccess = createTableService.up(createTable, context);
    if (!bSuccess) {return false;}

    QString sCopyQuery = QString("INSERT INTO %1 SELECT %2 FROM %3").arg(table.name()
                                                                         , table.joinedColumnNames()
                                                                         , tempTableName);
    bSuccess = CommandExecution::BaseCommandExecutionService::executeQuery(sCopyQuery, context);
    if (!bSuccess) {return false;}

    Commands::CommandPtr dropTable = Commands::CommandPtr(new Commands::DropTable(tempTableName));
    SqliteDropTableService dropTableService;
    bSuccess = dropTableService.up(dropTable, context);

    return bSuccess;
}

bool SqliteDropColumnService::isUpValid(const Commands::ConstCommandPtr &command
                                        , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());

    //check if table exists
    if (!context.database().tables().contains(dropColumn->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }
    return true;
}

bool SqliteDropColumnService::down(const Commands::ConstCommandPtr &command
                                   , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());
    SqliteAddColumnService addColumnService;
    return addColumnService.up(Commands::CommandPtr(new Commands::AddColumn(dropColumn->column()
                                                                            , dropColumn->tableName()))
                               , context);
    return false;
}

bool SqliteDropColumnService::isDownValid(const Commands::ConstCommandPtr &command
                                          , const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(context);

    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());
    if (!dropColumn->hasColumn()) {
        ::qWarning() << "Column information not complete! Migrating down is not possible!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
