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
#include "BaseSqlMigrator/CommandExecution/BaseSqlAlterColumnTypeService.h"

#include "Commands/AlterColumnType.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>

namespace CommandExecution {

BaseSqlAlterColumnTypeService::BaseSqlAlterColumnTypeService()
{
}

const QString &BaseSqlAlterColumnTypeService::commandType() const
{
    return Commands::AlterColumnType::typeName();
}

bool BaseSqlAlterColumnTypeService::execute(const Commands::ConstCommandPtr &command
                                       , CommandExecution::CommandExecutionContext &context
                                       ) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    Structure::Column originalColumn;
    bool success;
    originalColumn = context.helperAggregate()
            .dbReaderService->getTableDefinition(alterColumnType->tableName(), context.database())
            .fetchColumnByName(alterColumnType->columnName(), success);

    if (!success)
        return success; // failed, column doesn't exist

    QString newType;
    if (alterColumnType->hasSqlTypeString())
        newType = alterColumnType->newTypeString();
    else
        newType = context.helperAggregate().typeMapperService->map(alterColumnType->newType());

    QString alterQuery = QString("ALTER TABLE %1 ALTER COLUMN %2 SET DATA TYPE %3")
            .arg(context.helperAggregate().quoteService->quoteTableName(alterColumnType->tableName())
                 , context.helperAggregate().quoteService->quoteColumnName(alterColumnType->columnName())
                 , newType);

    success = CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);

    if (success && context.isUndoUsed()) {
        Commands::CommandPtr undoCommand(new Commands::AlterColumnType(alterColumnType->columnName()
                                                                       , alterColumnType->tableName()
                                                                       , originalColumn.sqlTypeString()
                                                                       , newType));
        context.setUndoCommand(undoCommand);
    }

    return success;
}

bool BaseSqlAlterColumnTypeService::isValid(const Commands::ConstCommandPtr &command
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

} // namespace CommandExecution
