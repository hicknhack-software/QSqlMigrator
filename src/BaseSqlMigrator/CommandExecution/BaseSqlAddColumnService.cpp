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
#include "BaseSqlMigrator/CommandExecution/BaseSqlAddColumnService.h"

#include "Commands/AddColumn.h"
#include "Commands/DropColumn.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

BaseSqlAddColumnService::BaseSqlAddColumnService()
{
}

const QString &BaseSqlAddColumnService::commandType() const
{
    return Commands::AddColumn::typeName();
}

bool BaseSqlAddColumnService::execute(const Commands::ConstCommandPtr &command
                                 , CommandExecution::CommandExecutionContext &context
                                 ) const
{
    QSharedPointer<const Commands::AddColumn> addColumn(command.staticCast<const Commands::AddColumn>());

    QString columnDefinition = context.helperAggregate().columnService->generateColumnDefinitionSql(addColumn->column());
    QString alterQuery = QString("ALTER TABLE %1 ADD COLUMN %2")
            .arg(context.helperAggregate().quoteService->quoteTableName(addColumn->tableName()), columnDefinition);

    bool success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(addColumn->reverse());
    }

    return success;
}

bool BaseSqlAddColumnService::isValid(const Commands::ConstCommandPtr &command
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
