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
#include "BaseSqlMigrator/CommandExecution/BaseSqlRenameTableService.h"

#include "Commands/RenameTable.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

BaseSqlRenameTableService::BaseSqlRenameTableService()
{
}

const QString &BaseSqlRenameTableService::commandType() const
{
    return Commands::RenameTable::typeName();
}

bool BaseSqlRenameTableService::execute(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameTable> renameTable(command.staticCast<const Commands::RenameTable>());

    QString alterQuery = QString("ALTER TABLE %1 RENAME TO %2")
            .arg(context.helperRepository().quoteService->quoteTableName(renameTable->name())
                 , context.helperRepository().quoteService->quoteTableName(renameTable->newName()));

    bool success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(renameTable->reverse());
    }

    return success;
}

bool BaseSqlRenameTableService::isValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameTable> renameTable(command.staticCast<const Commands::RenameTable>());

    //check if table exists
    if (!context.database().tables().contains(renameTable->name())) {
        ::qWarning() << "table not found!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
