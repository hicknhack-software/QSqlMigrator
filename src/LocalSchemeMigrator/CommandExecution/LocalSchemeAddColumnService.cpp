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
#include "LocalSchemeMigrator/CommandExecution/LocalSchemeAddColumnService.h"

#include "Commands/AddColumn.h"
#include "Commands/DropColumn.h"

#include "Structure/LocalScheme.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

LocalSchemeAddColumnService::LocalSchemeAddColumnService()
{
}

const QString &LocalSchemeAddColumnService::commandType() const
{
    return Commands::AddColumn::typeName();
}

bool LocalSchemeAddColumnService::execute(const Commands::ConstCommandPtr &command,
                                          CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AddColumn> addColumn(command.staticCast<const Commands::AddColumn>());

    const Structure::Table* table = context.localScheme()->table( addColumn->tableName() );
    if( nullptr == table ) {
        ::qWarning() << "table not found" << addColumn->tableName();
        return false;
    }

    Structure::Table::Builder alteredTable(table->name(), table->columns());
    alteredTable << addColumn->column();
    context.localScheme()->alterTable( alteredTable );

    return true;
}

bool LocalSchemeAddColumnService::isValid(const Commands::ConstCommandPtr &command,
                                          const CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::AddColumn> addColumn(command.staticCast<const Commands::AddColumn>());

    //check if table exists
    if (!context.localScheme()->tables().contains(addColumn->tableName())) {
        ::qWarning() << "table not found!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
