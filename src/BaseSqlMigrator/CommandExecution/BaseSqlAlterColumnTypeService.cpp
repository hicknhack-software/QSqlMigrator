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

#include "Helper/SqlStructureService.h"
#include "Helper/TypeMapperService.h"
#include "Helper/QuoteService.h"

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

bool BaseSqlAlterColumnTypeService::execute(const Commands::AlterColumnType &alterColumnType, const CommandExecutionContext &context)
{
    const QString alterQuery =
            QString("ALTER TABLE %1 ALTER COLUMN %2 SET DATA TYPE %3")
            .arg(context.helperRepository().quoteService().quoteTableName(alterColumnType.tableName()))
            .arg(context.helperRepository().quoteService().quoteColumnName(alterColumnType.columnName()))
            .arg(context.helperRepository().typeMapperService().map(alterColumnType.newType()));

    return CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);
}

bool BaseSqlAlterColumnTypeService::execute(const Commands::ConstCommandPtr &command,
                                            CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());
    Q_ASSERT(alterColumnType);

    const Structure::Column originalColumn( context.helperRepository().sqlStructureService()
                                            .getTableDefinition(alterColumnType->tableName(), context.database())
                                            .fetchColumnByName(alterColumnType->columnName()) );
    if (!originalColumn.isValid()) {
        ::qWarning() << "could not find column" << alterColumnType->tableName() << alterColumnType->columnName();
        return false;
    }

    bool success = execute(*alterColumnType, context);

    if (success && context.isUndoUsed()) {
        Commands::CommandPtr undoCommand(new Commands::AlterColumnType(alterColumnType->columnName(),
                                                                       alterColumnType->tableName(),
                                                                       originalColumn.type(), alterColumnType->newType()));
        context.setUndoCommand(undoCommand);
    }

    return success;
}

bool BaseSqlAlterColumnTypeService::isValid(const Commands::ConstCommandPtr &command,
                                            const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());

    Structure::Table table(context.helperRepository().sqlStructureService().getTableDefinition(alterColumnType->tableName(), context.database()));
    if (!table.isValid()) {
        ::qWarning() << "table doesn't exist!" << alterColumnType->tableName();
        return false;
    }
    if (!table.hasColumn(alterColumnType->columnName())) {
        ::qWarning() << "column doesn't exist!" << alterColumnType->tableName() << alterColumnType->columnName();
        return false;
    }
    return true;
}

} // namespace CommandExecution
