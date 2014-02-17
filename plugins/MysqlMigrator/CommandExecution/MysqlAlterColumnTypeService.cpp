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
#include "MysqlAlterColumnTypeService.h"

#include "Helper/QuoteService.h"
#include "Helper/ColumnService.h"
#include "Helper/SqlStructureService.h"

#include "MigrationTracker/MigrationTrackerService.h"

#include "Commands/AlterColumnType.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

MysqlAlterColumnTypeService::MysqlAlterColumnTypeService()
{
}

bool MysqlAlterColumnTypeService::execute(const Commands::AlterColumnType &alterColumnType, const Structure::Column &originalColumn, const CommandExecutionContext &context)
{
    Structure::Column modifiedColumn(originalColumn.name(), alterColumnType.newType(), originalColumn.defaultValue(), originalColumn.attributes());

    const QString alterQuery =
            QString("ALTER TABLE %1 MODIFY COLUMN %2")
            .arg(context.helperRepository().quoteService().quoteTableName(alterColumnType.tableName()))
            .arg(context.helperRepository().columnService().generateColumnDefinitionSql(modifiedColumn));

    return CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);
}

bool MysqlAlterColumnTypeService::execute(const Commands::ConstCommandPtr &command,
                                          CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AlterColumnType> alterColumnType(command.staticCast<const Commands::AlterColumnType>());
    Q_ASSERT( alterColumnType );

    const Structure::Table originalTable(context.helperRepository().sqlStructureService()
                                         .getTableDefinition(alterColumnType->tableName(), context.database()));
    bool success;
    const Structure::Column originalColumn( originalTable.fetchColumnByName( alterColumnType->columnName(), success ) );
    if (!success) {
        ::qWarning() << "could not find column" << alterColumnType->tableName() << alterColumnType->columnName();
        return false;
    }

    success = execute(*alterColumnType, originalColumn, context);

    if (success && context.isUndoUsed()) {
        Commands::CommandPtr undoCommand(new Commands::AlterColumnType(alterColumnType->columnName(), alterColumnType->tableName(),
                                                                       originalColumn.type(), alterColumnType->newType()));
        context.setUndoCommand(undoCommand);
    }
    return success;
}

} // namespace CommandExecution
