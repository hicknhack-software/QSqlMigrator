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
#include "SqliteMigrator/CommandExecution/SqliteAlterColumnService.h"

#include "BaseSqlMigrator/CommandExecution/BaseSqlRenameTableService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlCreateTableService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlDropTableService.h"

#include "Commands/DropTable.h"
#include "Commands/CreateTable.h"
#include "Commands/RenameTable.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

namespace CommandExecution {

SqliteAlterColumnService::SqliteAlterColumnService()
{
}

bool SqliteAlterColumnService::execute(const Structure::Table &origTable
                                       , const Structure::Table &newTable
                                       , CommandExecution::CommandExecutionContext &context) const
{
    QString tempTableName = QString("%1%2").arg(context.migrationConfig().temporaryTablePrefix
                                                , origTable.name());

    bool success;
    Commands::CommandPtr renameTable = Commands::CommandPtr(
                new Commands::RenameTable(origTable.name(), tempTableName));
    BaseSqlRenameTableService renameTableService;
    success = renameTableService.execute(renameTable, context);
    if (!success)
        return false;

    Commands::CommandPtr createTable = Commands::CommandPtr(new Commands::CreateTable(newTable));
    BaseSqlCreateTableService createTableService;
    success = createTableService.execute(createTable, context);
    if (!success)
        return false;

    //TODO check is "SELECT colX as colY" statement is necessary!
    QString copyQuery = QString("INSERT INTO %1 SELECT %2 FROM %3").arg(newTable.name()
                                                                         , origTable.joinedColumnNames()
                                                                         , tempTableName);
    ::qDebug() << "complete query-string looks like:";
    ::qDebug() << copyQuery;
    QSqlQuery query = context.database().exec(copyQuery);
    QSqlError error = query.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
        return false;
    }

    Commands::CommandPtr dropTable = Commands::CommandPtr(new Commands::DropTable(tempTableName));
    BaseSqlDropTableService dropTableService;
    success = dropTableService.execute(dropTable, context);

    return success;
}

} // namespace CommandExecution
