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
#include "LocalSchemeMigrator/CommandExecution/LocalSchemeDropColumnService.h"

#include "Commands/DropColumn.h"
#include "Commands/AddColumn.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

LocalSchemeDropColumnService::LocalSchemeDropColumnService()
{}

const QString &LocalSchemeDropColumnService::commandType() const
{
    return Commands::DropColumn::typeName();
}

bool LocalSchemeDropColumnService::execute(const Commands::ConstCommandPtr &command
                                  , CommandExecution::LocalSchemeCommandExecutionContext &context
                                  ) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());

    Structure::ColumnList list = context.localScheme()->tables()[dropColumn->tableName()].columns();
    int i=0;
    foreach (const Structure::Column &column, list) {
        if (column.name() == dropColumn->columnName()) {
            list.removeAt(i);
            break;
        }
        i++;
    }
    context.localScheme()->tables().insert(dropColumn->tableName(), Structure::Table(dropColumn->tableName(), list));

    return true;
}

bool LocalSchemeDropColumnService::isValid(const Commands::ConstCommandPtr &command
                                        , const CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropColumn> dropColumn(command.staticCast<const Commands::DropColumn>());

    //check if table exists
    if (!context.localScheme()->tables().contains(dropColumn->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
