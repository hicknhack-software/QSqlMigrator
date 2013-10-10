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
#include "BaseSqlMigrator/CommandExecution/BaseSqlCreateIndexService.h"

#include "Commands/CreateIndex.h"
#include "Commands/DropIndex.h"

#include <QDebug>

namespace CommandExecution {

BaseSqlCreateIndexService::BaseSqlCreateIndexService()
{
}

const QString &BaseSqlCreateIndexService::commandType() const
{
    return Commands::CreateIndex::typeName();
}

bool BaseSqlCreateIndexService::execute(const Commands::ConstCommandPtr &command
                                   , CommandExecution::CommandExecutionContext &context
                                   ) const
{
    QSharedPointer<const Commands::CreateIndex> createIndex(command.staticCast<const Commands::CreateIndex>());

    QString columnDefinition = context.helperAggregate().columnService->generateIndexColumnDefinitionSql(createIndex->index().columns());

    QString createQuery = QString("CREATE INDEX %1 ON %2 (%3)")
            .arg(context.helperAggregate().quoteService->quoteTableName(createIndex->index().name())
                 , context.helperAggregate().quoteService->quoteTableName(createIndex->index().tableName())
                 , columnDefinition);

    bool success = CommandExecution::BaseCommandExecutionService::executeQuery(createQuery, context);

    if (success && context.isUndoUsed()) {
        context.setUndoCommand(createIndex->reverse());
    }

    return success;
}

bool BaseSqlCreateIndexService::isValid(const Commands::ConstCommandPtr &command
                                         , const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(context);
    QSharedPointer<const Commands::CreateIndex> createIndex(command.staticCast<const Commands::CreateIndex>());

    if (createIndex->index().columns().size() < 1) {
        ::qWarning() << "at least one Column name must be specified to create an index!";
        return false;
    }
    return true;
}

} // namespace CommandExecution
