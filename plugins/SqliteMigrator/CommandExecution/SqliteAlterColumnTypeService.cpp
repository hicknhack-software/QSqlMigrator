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
#include "SqliteMigrator/CommandExecution/SqliteAlterColumnTypeService.h"

#include "SqliteMigrator/CommandExecution/SqliteAlterColumnService.h"
#include "SqliteMigrator/Helper/SqliteDbReader.h"

#include "Commands/AlterColumnType.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

SqliteAlterColumnTypeService::SqliteAlterColumnTypeService()
{
}

const QString &SqliteAlterColumnTypeService::commandType() const
{
    return Commands::AlterColumnType::typeName();
}

bool SqliteAlterColumnTypeService::up(const Commands::ConstCommandPtr &command
                                      , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    Helper::SqliteDbReader dbReader;
    Structure::Table origTable = dbReader.getTableDefinition(alterColumnType->tableName(), context);
    Structure::Table newTable = Structure::Table(alterColumnType->tableName());
    foreach (Structure::Column column, origTable.columns()) {
        if (column.name() == alterColumnType->columnName()) {
            Structure::Column newColumn = Structure::Column(column.name(), alterColumnType->newType(), column.attributes());
            newTable.add(newColumn);
        } else {
            newTable.add(column);
        }
    }

    SqliteAlterColumnService alterColumnService;
    return alterColumnService.run(origTable, newTable, context);
}

bool SqliteAlterColumnTypeService::isUpValid(const Commands::ConstCommandPtr &command
                                             , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    //check if table exists
    if (!context.database().tables().contains(alterColumnType->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }
    return true;
}

bool SqliteAlterColumnTypeService::down(const Commands::ConstCommandPtr &command
                                        , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());
    return this->up(Commands::CommandPtr(new Commands::AlterColumnType(alterColumnType->columnName()
                                                                       , alterColumnType->tableName()
                                                                       , alterColumnType->oldType()))
                    , context);
}

bool SqliteAlterColumnTypeService::isDownValid(const Commands::ConstCommandPtr &command
                                               , const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(context);

    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    if (!alterColumnType->hasOldType()) {
        ::qWarning() << "No oldType defined. Down-migration is not possible!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
