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
#include "BaseSqlMigrator/CommandExecution/BaseSqlRenameColumnService.h"

#include "Commands/RenameColumn.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>
#include <QScopedPointer>

using namespace Structure;

namespace CommandExecution {

BaseSqlRenameColumnService::BaseSqlRenameColumnService()
{
}

const QString &BaseSqlRenameColumnService::commandType() const
{
    return Commands::RenameColumn::typeName();
}

bool BaseSqlRenameColumnService::execute(const Commands::ConstCommandPtr &command
                                   , CommandExecution::CommandExecutionContext &context
                                    ) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());

    Structure::Table origTable = context.helperAggregate().dbReaderService->getTableDefinition(renameColumn->tableName(), context.database());
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

    QString columnDefinition = context.helperAggregate().columnService->generateColumnDefinitionSql(*modifiedColumn);

    QString alterQuery = QString("ALTER TABLE %1 CHANGE COLUMN %2 %3")
            .arg(context.helperAggregate().quoteService->quoteTableName(renameColumn->tableName())
                 , renameColumn->name()
                 , columnDefinition);

    bool success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(renameColumn->reverse());
    }

    return success;
}

bool BaseSqlRenameColumnService::isValid(const Commands::ConstCommandPtr &command
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

} // namespace CommandExecution
