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
#include "MysqlMigrator/CommandExecution/MysqlRenameColumnService.h"

#include "MysqlMigrator/Helper/MysqlColumnService.h"
#include "MysqlMigrator/Helper/MysqlDbReader.h"
#include "MysqlMigrator/Helper/MysqlQuoteService.h"

#include "Commands/RenameColumn.h"

#include "Structure/Column.h"
#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>
#include <QScopedPointer>

namespace CommandExecution {

MysqlRenameColumnService::MysqlRenameColumnService()
{
}

const QString &MysqlRenameColumnService::commandType() const
{
    return Commands::RenameColumn::typeName();
}

bool MysqlRenameColumnService::up(const Commands::ConstCommandPtr &command
                                  , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());

    Helper::MysqlDbReader dbReader;
    Structure::Table origTable = dbReader.getTableDefinition(renameColumn->tableName(), context);
    QScopedPointer<Structure::Column> modifiedColumn;
    foreach (Structure::Column column, origTable.columns()) {
        if (column.name() == renameColumn->name()) {
            modifiedColumn.reset(new Structure::Column(renameColumn->newName(), column.sqlType(), column.attributes()));
            if (column.hasDefaultValue()) {
                modifiedColumn->setDefault(column.defaultValue());
            }
            break;
        }
    }

    Helper::MysqlColumnService columnService;
    QString columnDefinition = columnService.generateColumnDefinitionSql(*modifiedColumn);

    QString alterQuery = QString("ALTER TABLE %1 CHANGE COLUMN %2 %3")
            .arg(Helper::MysqlQuoteService::quoteTableName(renameColumn->tableName())
                 , renameColumn->name()
                 , columnDefinition);

    bool success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(Commands::CommandPtr(new Commands::RenameColumn(
                                                        renameColumn->newName()
                                                        , renameColumn->name()
                                                        , renameColumn->tableName())));
    }

    return success;
}

bool MysqlRenameColumnService::isUpValid(const Commands::ConstCommandPtr &command
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

bool MysqlRenameColumnService::down(const Commands::ConstCommandPtr &command
                                    , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());
    return this->up(Commands::CommandPtr(new Commands::RenameColumn(renameColumn->newName()
                                                                    , renameColumn->name()
                                                                    , renameColumn->tableName()))
                    , context);
}

bool MysqlRenameColumnService::isDownValid(const Commands::ConstCommandPtr &command
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
