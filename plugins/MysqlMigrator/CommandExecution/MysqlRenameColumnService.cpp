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
#include "MysqlRenameColumnService.h"

#include "MigrationTracker/MigrationTrackerService.h"

#include "Helper/QuoteService.h"
#include "Helper/ColumnService.h"
#include "Helper/SqlStructureService.h"

#include "Commands/RenameColumn.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>
#include <QScopedPointer>

using namespace Structure;

namespace CommandExecution {

MysqlRenameColumnService::MysqlRenameColumnService()
{
}

bool MysqlRenameColumnService::execute(const Commands::RenameColumn &renameColumn, const Structure::Column &originalColumn, const CommandExecutionContext &context)
{
    Structure::Column modifiedColumn(renameColumn.newName(), originalColumn.type(), originalColumn.defaultValue(), originalColumn.attributes());

    const QString alterQuery =
            QString("ALTER TABLE %1 CHANGE COLUMN %2 %3")
            .arg(context.helperRepository().quoteService().quoteTableName(renameColumn.tableName()))
            .arg(renameColumn.oldName())
            .arg(context.helperRepository().columnService().generateColumnDefinitionSql(modifiedColumn));

    return CommandExecution::BaseCommandExecutionService::executeQuery(alterQuery, context);
}

bool MysqlRenameColumnService::execute(const Commands::ConstCommandPtr &command,
                                       CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());
    Q_ASSERT(renameColumn);

    const Structure::Column originalColumn( context.helperRepository().sqlStructureService()
                                            .getTableDefinition(renameColumn->tableName(), context.database())
                                            .fetchColumnByName(renameColumn->oldName()) );
    if (!originalColumn.isValid()) {
        ::qWarning() << "could not find column" << renameColumn->tableName() << renameColumn->oldName();
        return false;
    }

    bool success = execute(*renameColumn, originalColumn, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(renameColumn->reverse());
    }
    return success;
}

} // namespace CommandExecution
