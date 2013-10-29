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

bool BaseSqlDropColumnService::execute(const Commands::ConstCommandPtr &command
                                  , CommandExecution::CommandExecutionContext &context
                                  ) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());

    Structure::Column originalColumn;
    bool success;
    originalColumn = context.helperRepository()
            .dbReaderService->getTableDefinition(dropColumn->tableName(), context.database())
            .fetchColumnByName(dropColumn->columnName(), success);

    if (!success)
        return success; // failed, column doesn't exist

    QString alterQuery = QString("ALTER TABLE %1 DROP COLUMN %2")
            .arg(context.helperRepository().quoteService->quoteTableName(dropColumn->tableName())
                 , context.helperRepository().quoteService->quoteColumnName(dropColumn->columnName()));

    success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(Commands::CommandPtr(new Commands::AddColumn(originalColumn
                                                                            , dropColumn->tableName())));
    }

    return success;
}

bool BaseSqlDropColumnService::isValid(const Commands::ConstCommandPtr &command
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

} // namespace CommandExecution
