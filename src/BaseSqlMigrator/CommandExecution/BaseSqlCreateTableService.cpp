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
#include "BaseSqlMigrator/CommandExecution/BaseSqlCreateTableService.h"

#include "Commands/CreateTable.h"
#include "Commands/DropTable.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

BaseSqlCreateTableService::BaseSqlCreateTableService()
{
}

const QString &BaseSqlCreateTableService::commandType() const
{
    return Commands::CreateTable::typeName();
}

bool BaseSqlCreateTableService::execute(const Commands::ConstCommandPtr &command
                                   , CommandExecution::CommandExecutionContext &context
                                   ) const
{
    QSharedPointer<const Commands::CreateTable> createTable(command.staticCast<const Commands::CreateTable>());

    QString columnDefinition = context.helperAggregate().columnService->generateColumnsDefinitionSql(createTable->table().columns());
    QString createQuery = QString("CREATE TABLE %1 (%2)")
            .arg(context.helperAggregate().quoteService->quoteTableName(createTable->table().name()), columnDefinition);

    bool success = CommandExecution::BaseCommandExecutionService::executeQuery(createQuery, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(createTable->reverse());
    }

    return success;
}


bool BaseSqlCreateTableService::isValid(const Commands::ConstCommandPtr &command
                                         , const CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateTable> createTable(command.staticCast<const Commands::CreateTable>());

    //check if table allready exists
    if (context.database().tables().contains(createTable->table().name())) {
        ::qWarning() << "table allready exists!";
        return false;
    }

    //check for columndefinition, at least 1 column needed!
    if (createTable->table().columns().size() < 1) {
        ::qWarning() << "need, at least 1 column, to create a new table!";
        return false;
    }
    return true;
}

} // namespace BaseSqlCommandService
