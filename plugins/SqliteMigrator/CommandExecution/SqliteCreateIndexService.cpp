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
#include "SqliteMigrator/CommandExecution/SqliteCreateIndexService.h"

#include "SqliteMigrator/CommandExecution/SqliteDropIndexService.h"
#include "SqliteMigrator/Helper/SqliteColumnService.h"

#include "Commands/CreateIndex.h"
#include "Commands/DropIndex.h"

#include <QDebug>

namespace CommandExecution {

SqliteCreateIndexService::SqliteCreateIndexService()
{
}

const QString &SqliteCreateIndexService::commandType() const
{
    return Commands::CreateIndex::typeName();
}

bool SqliteCreateIndexService::up(const Commands::ConstCommandPtr &command
                                  , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateIndex> createIndex(command.staticCast<const Commands::CreateIndex>());

    Helper::SqliteColumnService columnService;
    QString sColumnDefinition = columnService.generateIndexColumnDefinitionSql(createIndex->index().columns());

    QString sCreateQuery = QString("CREATE INDEX %1 ON %2 (%3)")
            .arg(createIndex->index().name(), createIndex->index().tableName(), sColumnDefinition);

    return CommandExecution::BaseCommandExecutionService::executeQuery(sCreateQuery, context);
}

bool SqliteCreateIndexService::isUpValid(const Commands::ConstCommandPtr &command
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

bool SqliteCreateIndexService::down(const Commands::ConstCommandPtr &command
                                    , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateIndex> createIndex(command.staticCast<const Commands::CreateIndex>());
    SqliteDropIndexService dropIndexService;
    return dropIndexService.up(Commands::CommandPtr(new Commands::DropIndex(createIndex->name())), context);
}

bool SqliteCreateIndexService::isDownValid(const Commands::ConstCommandPtr &command
                                           , const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(command);
    Q_UNUSED(context);
    return true;
}

} // namespace CommandExecution
