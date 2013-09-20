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
#include "SqliteMigrator/CommandExecution/SqliteAddColumnService.h"

#include "SqliteMigrator/CommandExecution/SqliteDropColumnService.h"
#include "SqliteMigrator/Helper/SqliteColumnService.h"

#include "Commands/AddColumn.h"
#include "Commands/DropColumn.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteAddColumnService::SqliteAddColumnService()
{
}

const QString &SqliteAddColumnService::commandType() const
{
    return Commands::AddColumn::typeName();
}

bool SqliteAddColumnService::up(const Commands::ConstCommandPtr &command
                                , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AddColumn> addColumn(command.staticCast<const Commands::AddColumn>());

    Helper::SqliteColumnService columnService;
    QString columnDefinition = columnService.generateColumnDefinitionSql(addColumn->column());
    QString alterQuery = QString("ALTER TABLE %1 ADD COLUMN %2").arg(addColumn->tableName(), columnDefinition);

    return CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);
}

bool SqliteAddColumnService::isUpValid(const Commands::ConstCommandPtr &command
                                       , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AddColumn> addColumn(command.staticCast<const Commands::AddColumn>());

    //check if table exists
    if (!context.database().tables().contains(addColumn->tableName())) {
        ::qWarning() << "table not found!";
        return false;
    }
    return true;
}

bool SqliteAddColumnService::down(const Commands::ConstCommandPtr &command
                                  , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AddColumn> addColumn(command.staticCast<const Commands::AddColumn>());

    SqliteDropColumnService dropColumnService;
    return dropColumnService.up(Commands::CommandPtr(new Commands::DropColumn(addColumn->column().name()
                                                                              , addColumn->tableName()))
                                , context);
}

bool SqliteAddColumnService::isDownValid(const Commands::ConstCommandPtr &command
                                         , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AddColumn> addColumn(command.staticCast<const Commands::AddColumn>());

    //check if table exists
    if (!context.database().tables().contains(addColumn->tableName())) {
        ::qWarning() << "table not found!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
