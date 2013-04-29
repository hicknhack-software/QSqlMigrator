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
#include "SqliteCreateTableService.h"
#include "SqliteDropTableService.h"
#include "Commands/CreateTable.h"
#include "Commands/DropTable.h"
#include "Helper/SqliteColumnService.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteCreateTableService::SqliteCreateTableService()
{
}

const QString &SqliteCreateTableService::commandType() const
{
    return Commands::CreateTable::typeName();
}

bool SqliteCreateTableService::up(const Commands::ConstCommandPtr &command
                                  , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateTable> createTable(command.staticCast<const Commands::CreateTable>());

    Helper::SqliteColumnService columnService;
    QString sColumnDefinition = columnService.generateColumnDefinitionSql(createTable->table().columns());
    QString sCreateQuery = QString("CREATE TABLE %1 (%2)").arg(createTable->table().name(), sColumnDefinition);

    return CommandExecution::BaseCommandExecutionService::executeQuery(sCreateQuery, context);
}


bool SqliteCreateTableService::isUpValid(const Commands::ConstCommandPtr &command
                                         , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateTable> createTable(command.staticCast<const Commands::CreateTable>());

    //check if table allready exists
    if (context.database().tables().contains(createTable->table().name())) {
        ::qWarning() << "table allready exists!";
        return false;
    }

    //check for columndefinition, at least 1 column needed!
    if (createTable->table().columns().size() < 1) {
        ::qWarning() << "need, at least 1 column, to create a new table!";
        return false;
    }
    return true;
}

bool SqliteCreateTableService::down(const Commands::ConstCommandPtr &command
                                    , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateTable> createTable(command.staticCast<const Commands::CreateTable>());
    SqliteDropTableService dropTableService;
    return dropTableService.up(Commands::CommandPtr(new Commands::DropTable(createTable->table().name())), context);
}

bool SqliteCreateTableService::isDownValid(const Commands::ConstCommandPtr &command
                                           , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateTable> createTable(command.staticCast<const Commands::CreateTable>());

    //check if table allready exists
    if (!context.database().tables().contains(createTable->table().name())) {
        ::qWarning() << "table does not exist!";
        return false;
    }
    return true;
}

} // namespace SqliteCommandService
