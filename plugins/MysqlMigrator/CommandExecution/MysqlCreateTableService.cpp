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
#include "MysqlMigrator/CommandExecution/MysqlCreateTableService.h"
#include "MysqlMigrator/CommandExecution/MysqlDropTableService.h"

#include "MysqlMigrator/Helper/MysqlColumnService.h"
#include "MysqlMigrator/Helper/MysqlQuoteService.h"

#include "Commands/CreateTable.h"
#include "Commands/DropTable.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

namespace CommandExecution {

MysqlCreateTableService::MysqlCreateTableService()
{
}

const QString &MysqlCreateTableService::commandType() const
{
    return Commands::CreateTable::typeName();
}

bool MysqlCreateTableService::up(const Commands::ConstCommandPtr &command, CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateTable> createTable(command.staticCast<const Commands::CreateTable>());

    Helper::MysqlColumnService columnService;
    QString columnDefinition = columnService.generateColumnDefinitionSql(createTable->table().columns());

    QString createQuery = QString("CREATE TABLE %1 (%2)")
            .arg(Helper::MysqlQuoteService::quoteTableName(createTable->table().name())
                 , columnDefinition);

    bool success = CommandExecution::BaseCommandExecutionService::executeQuery(createQuery, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(Commands::CommandPtr(new Commands::DropTable(createTable->table())));
    }

    return success;
}

bool MysqlCreateTableService::isUpValid(const Commands::ConstCommandPtr &command, const CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateTable> createTable(command.staticCast<const Commands::CreateTable>());

    if (context.database().tables().contains(createTable->table().name())) {
        ::qWarning() << "table allready exists!";
        return false;
    }

    if (createTable->table().columns().size() < 1) {
        ::qWarning() << "need, at least 1 column, to create a new table!";
        return false;
    }
    return true;
}

bool MysqlCreateTableService::down(const Commands::ConstCommandPtr &command
                                   , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateTable> createTable(command.staticCast<const Commands::CreateTable>());
    MysqlDropTableService dropTableService;
    return dropTableService.up(Commands::CommandPtr(new Commands::DropTable(createTable->table().name())), context);
}

bool MysqlCreateTableService::isDownValid(const Commands::ConstCommandPtr &command
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

} // namespace CommandExecution
