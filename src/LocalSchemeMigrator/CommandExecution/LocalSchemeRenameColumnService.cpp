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
#include "LocalSchemeMigrator/CommandExecution/LocalSchemeRenameColumnService.h"

#include "Commands/RenameColumn.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>
#include <QScopedPointer>

using namespace Structure;

namespace CommandExecution {

LocalSchemeRenameColumnService::LocalSchemeRenameColumnService()
{
}

const QString &LocalSchemeRenameColumnService::commandType() const
{
    return Commands::RenameColumn::typeName();
}

bool LocalSchemeRenameColumnService::execute(const Commands::ConstCommandPtr &command,
                                             CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());

    const Structure::Table* table = context.localScheme()->table( renameColumn->tableName() );
    if( nullptr == table ) {
        ::qWarning() << "table not found" << renameColumn->tableName();
        return false;
    }

    Structure::Table::Builder alteredTable(table->name(), table->columns());
    bool found = false;
    foreach (const Structure::Column &column, table->columns()) {
        if (column.name() == renameColumn->name()) {
            alteredTable << Structure::Column( renameColumn->newName(), column.type(), column.defaultValue(), column.attributes() );
            found = true;
        }
        else {
            alteredTable << column;
        }
    }
    if (!found) {
        ::qWarning() << "column not found" << renameColumn->tableName() << renameColumn->name();
        return false;
    }
    context.localScheme()->alterTable( alteredTable );

    return true;
}

bool LocalSchemeRenameColumnService::isValid(const Commands::ConstCommandPtr &command
                                          , const CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::RenameColumn> renameColumn(command.staticCast<const Commands::RenameColumn>());

    //check if table exists
    if (!context.localScheme()->tables().contains(renameColumn->tableName())) {
        ::qWarning() << "table doesn't exist!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
