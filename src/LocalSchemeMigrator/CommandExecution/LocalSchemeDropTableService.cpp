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
#include "LocalSchemeMigrator/CommandExecution/LocalSchemeDropTableService.h"

#include "Commands/DropTable.h"
#include "Commands/CreateTable.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

LocalSchemeDropTableService::LocalSchemeDropTableService()
{
}

const QString &LocalSchemeDropTableService::commandType() const
{
    return Commands::DropTable::typeName();
}

bool LocalSchemeDropTableService::execute(const Commands::ConstCommandPtr &command
                                 , CommandExecution::LocalSchemeCommandExecutionContext &context
                                 ) const
{
    QSharedPointer<const Commands::DropTable> dropTable(command.staticCast<const Commands::DropTable>());

    context.localScheme()->tables().remove(dropTable->tableName());

    return true;
}

bool LocalSchemeDropTableService::isValid(const Commands::ConstCommandPtr &command
                                          , const CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropTable> dropTable(command.staticCast<const Commands::DropTable>());

    //check if tableName is empty
    if (dropTable->tableName().isEmpty()) {
        ::qWarning() << "need to specify a table name!";
        return false;
    }

    //check if table exists
    if (!context.localScheme()->tables().contains(dropTable->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }

    return true;
}

} // namespace CommandExecution
