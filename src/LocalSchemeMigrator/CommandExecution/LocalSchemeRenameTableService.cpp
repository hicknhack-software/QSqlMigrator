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
#include "LocalSchemeMigrator/CommandExecution/LocalSchemeRenameTableService.h"

#include "Commands/RenameTable.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

LocalSchemeRenameTableService::LocalSchemeRenameTableService()
{
}

const QString &LocalSchemeRenameTableService::commandType() const
{
    return Commands::RenameTable::typeName();
}

bool LocalSchemeRenameTableService::execute(const Commands::ConstCommandPtr &command
                                            , CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameTable> renameTable(command.staticCast<const Commands::RenameTable>());

    Structure::Table origTable = context.localScheme()->tables()[renameTable->name()];
    context.localScheme()->tables().remove(renameTable->name());
    context.localScheme()->tables().insert(renameTable->newName(), Structure::Table(renameTable->newName(), origTable.columns()));

    return true;
}

bool LocalSchemeRenameTableService::isValid(const Commands::ConstCommandPtr &command
                                            , const CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameTable> renameTable(command.staticCast<const Commands::RenameTable>());

    //check if table exists
    if (!context.localScheme()->tables().contains(renameTable->name())) {
        ::qWarning() << "table not found!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
