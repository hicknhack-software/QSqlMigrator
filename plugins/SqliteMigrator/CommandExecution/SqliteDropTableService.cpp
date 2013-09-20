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
#include "SqliteMigrator/CommandExecution/SqliteDropTableService.h"

#include "SqliteMigrator/CommandExecution/SqliteCreateTableService.h"
#include "SqliteMigrator/Helper/SqliteDbReader.h"

#include "Commands/CreateTable.h"
#include "Commands/DropTable.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteDropTableService::SqliteDropTableService()
{
}

const QString &SqliteDropTableService::commandType() const
{
    return Commands::DropTable::typeName();
}

bool SqliteDropTableService::up(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropTable> dropTable(command.staticCast<const Commands::DropTable>());
    QString dropQuery = QString("DROP TABLE %1").arg(dropTable->tableName());

    return CommandExecution::BaseCommandExecutionService::executeQuery(dropQuery, context);
}

bool SqliteDropTableService::isUpValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropTable> dropTable(command.staticCast<const Commands::DropTable>());

    //check if tableName is empty
    if (dropTable->tableName().isEmpty()) {
        ::qWarning() << "need to specify a table name!";
        return false;
    }

    //check if table exists
    if (!context.database().tables().contains(dropTable->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }

    return true;
}

bool SqliteDropTableService::down(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropTable> dropTable(command.staticCast<const Commands::DropTable>());
    SqliteCreateTableService createTableService;
    return createTableService.up(Commands::CommandPtr(new Commands::CreateTable(dropTable->table()))
                                 , context);
}

bool SqliteDropTableService::isDownValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(context);

    QSharedPointer<const Commands::DropTable> dropTable(command.staticCast<const Commands::DropTable>());
    if (!dropTable->hasTable()) {
        ::qWarning() << "Table information not complete. Migrating down is not possible!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
