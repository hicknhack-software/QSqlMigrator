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
#include "BaseSqlMigrator/CommandExecution/BaseSqlDropColumnService.h"

#include "Helper/SqlStructureService.h"
#include "Helper/QuoteService.h"

#include "Commands/DropColumn.h"
#include "Commands/AddColumn.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

BaseSqlDropColumnService::BaseSqlDropColumnService()
{}

const QString &BaseSqlDropColumnService::commandType() const
{
    return Commands::DropColumn::typeName();
}

bool BaseSqlDropColumnService::execute(const Commands::DropColumn &dropColumn, const CommandExecutionContext &context)
{
    const QString alterQuery =
            QString("ALTER TABLE %1 DROP COLUMN %2")
            .arg(context.helperRepository().quoteService().quoteTableName(dropColumn.tableName()))
            .arg(context.helperRepository().quoteService().quoteColumnName(dropColumn.columnName()));

    return CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);
}

bool BaseSqlDropColumnService::execute(const Commands::ConstCommandPtr &command,
                                       CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());
    Q_ASSERT(dropColumn);

    const Structure::Column originalColumn( context.helperRepository().sqlStructureService()
                                            .getTableDefinition(dropColumn->tableName(), context.database())
                                            .fetchColumnByName(dropColumn->columnName()) );

    bool success = execute(*dropColumn, context);

    if (success && context.isUndoUsed()) {
        Commands::CommandPtr undoCommand(new Commands::AddColumn(originalColumn, dropColumn->tableName()));
        context.setUndoCommand(undoCommand);
        return true;
    }

    return success;
}

bool BaseSqlDropColumnService::isValid(const Commands::ConstCommandPtr &command,
                                       const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());

    Structure::Table table(context.helperRepository().sqlStructureService().getTableDefinition(dropColumn->tableName(), context.database()));
    if (!table.isValid()) {
        ::qWarning() << "table doesn't exist!" << dropColumn->tableName();
        return false;
    }
    if (!table.hasColumn(dropColumn->columnName())) {
        ::qWarning() << "column doesn't exist!" << dropColumn->tableName() << dropColumn->columnName();
        return false;
    }
    return true;
}

} // namespace CommandExecution
