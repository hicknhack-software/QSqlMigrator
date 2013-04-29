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
#include "MysqlDropColumnService.h"
#include "MysqlAddColumnService.h"

#include "Commands/AddColumn.h"
#include "Commands/DropColumn.h"

#include "Helper/MysqlDbReader.h"
#include "Helper/MysqlQuoteService.h"

#include "Structure/Column.h"
#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

MysqlDropColumnService::MysqlDropColumnService()
{
}

const QString &MysqlDropColumnService::commandType() const
{
    return Commands::DropColumn::typeName();
}

bool MysqlDropColumnService::up(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());

    Helper::MysqlDbReader dbReader;
    Structure::Table origTable = dbReader.getTableDefinition(dropColumn->tableName(), context);
    Structure::Column *originalColumn;
    foreach (Structure::Column column, origTable.columns()) {
        if (column.name() == dropColumn->columnName()) {
            originalColumn = new Structure::Column(column.name(), column.sqlType(), column.attributes());
            if (column.hasDefaultValue()) {
                originalColumn->setDefault(column.defaultValue());
            }
            break;
        }
    }

    QString sAlterQuery = QString("ALTER TABLE %1 DROP COLUMN %2")
            .arg(Helper::MysqlQuoteService::quoteTableName(dropColumn->tableName())
                 , Helper::MysqlQuoteService::quoteColumnName(dropColumn->columnName()));

    bool bSuccess = CommandExecution::BaseCommandExecutionService::executeQuery(sAlterQuery, context);

    if (bSuccess && context.isUndoUsed()) {
        context.setUndoCommand(Commands::CommandPtr(new Commands::AddColumn(*originalColumn
                                                                            , dropColumn->tableName())));
    }

    return bSuccess;
}

bool MysqlDropColumnService::isUpValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());

    //check if table exists
    if (!context.database().tables().contains(dropColumn->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }
    return true;
}

bool MysqlDropColumnService::down(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());
    MysqlAddColumnService addColumnService;
    Commands::CommandPtr addColumnCommand(new Commands::AddColumn(dropColumn->column()
                                                                  , dropColumn->tableName()));
    return addColumnService.up(addColumnCommand, context);
    return false;
}

bool MysqlDropColumnService::isDownValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
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
