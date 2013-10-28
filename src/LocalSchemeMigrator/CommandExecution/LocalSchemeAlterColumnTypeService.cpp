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
#include "LocalSchemeMigrator/CommandExecution/LocalSchemeAlterColumnTypeService.h"

#include "Commands/AlterColumnType.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>

namespace CommandExecution {

LocalSchemeAlterColumnTypeService::LocalSchemeAlterColumnTypeService()
{
}

const QString &LocalSchemeAlterColumnTypeService::commandType() const
{
    return Commands::AlterColumnType::typeName();
}

bool LocalSchemeAlterColumnTypeService::execute(const Commands::ConstCommandPtr &command
                                       , CommandExecution::LocalSchemeCommandExecutionContext &context
                                       ) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    Structure::ColumnList list = context.localScheme()->tables()[alterColumnType->tableName()].columns();
    int i=0;
    foreach (const Structure::Column &column, list) {
        if (column.name() == alterColumnType->columnName()) {
            if (alterColumnType->hasSqlTypeString())
                list.replace(i, Structure::Column(column.name(), alterColumnType->newTypeString(), column.defaultValue(), column.attributes()));
            else
                list.replace(i, Structure::Column(column.name(), alterColumnType->newType(), column.defaultValue(), column.attributes()));
            break;
        }
        i++;
    }
    context.localScheme()->tables().insert(alterColumnType->tableName(), Structure::Table(alterColumnType->tableName(), list));

    return true;
}

bool LocalSchemeAlterColumnTypeService::isValid(const Commands::ConstCommandPtr &command
                                             , const CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    //check if table exists
    if (!context.localScheme()->tables().contains(alterColumnType->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
