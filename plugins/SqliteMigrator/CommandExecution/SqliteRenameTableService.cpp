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
#include "SqliteRenameTableService.h"

#include "Commands/RenameTable.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteRenameTableService::SqliteRenameTableService()
{
}

const QString &SqliteRenameTableService::commandType() const
{
    return Commands::RenameTable::typeName();
}

bool SqliteRenameTableService::up(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameTable> renameTable(command.staticCast<const Commands::RenameTable>());

    QString sAlterQuery = QString("ALTER TABLE %1 RENAME TO %2").arg(renameTable->name(), renameTable->newName());

    return CommandExecution::BaseCommandExecutionService::executeQuery(sAlterQuery, context);
}

bool SqliteRenameTableService::isUpValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameTable> renameTable(command.staticCast<const Commands::RenameTable>());

    //check if table exists
    if (!context.database().tables().contains(renameTable->name())) {
        ::qWarning() << "table not found!";
        return false;
    }
    return true;
}

bool SqliteRenameTableService::down(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameTable> renameTable(command.staticCast<const Commands::RenameTable>());
    return this->up(
                Commands::CommandPtr(new Commands::RenameTable(renameTable->newName(), renameTable->name()))
                , context);
}

bool SqliteRenameTableService::isDownValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameTable> renameTable(command.staticCast<const Commands::RenameTable>());

    //check if table exists
    if (!context.database().tables().contains(renameTable->newName())) {
        ::qWarning() << "table not found!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
