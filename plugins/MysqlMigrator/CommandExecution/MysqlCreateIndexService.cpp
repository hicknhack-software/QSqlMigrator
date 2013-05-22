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
#include "MysqlMigrator/CommandExecution/MysqlCreateIndexService.h"
#include "MysqlMigrator/CommandExecution/MysqlDropIndexService.h"

#include "MysqlMigrator/Helper/MysqlColumnService.h"
#include "MysqlMigrator/Helper/MysqlQuoteService.h"

#include "Commands/CreateIndex.h"
#include "Commands/DropIndex.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

MysqlCreateIndexService::MysqlCreateIndexService()
{
}

const QString &MysqlCreateIndexService::commandType() const
{
    return Commands::CreateIndex::typeName();
}

bool MysqlCreateIndexService::up(const Commands::ConstCommandPtr &command
                                 , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateIndex> createIndex(command.staticCast<const Commands::CreateIndex>());

    Helper::MysqlColumnService columnService;
    QString sColumnDefinition = columnService.generateIndexColumnDefinitionSql(createIndex->index().columns());

    QString sCreateQuery = QString("CREATE INDEX %1 ON %2 (%3)")
            .arg(Helper::MysqlQuoteService::quoteTableName(createIndex->index().name())
                 , Helper::MysqlQuoteService::quoteTableName(createIndex->index().tableName())
                 , sColumnDefinition);

    return CommandExecution::BaseCommandExecutionService::executeQuery(sCreateQuery, context);
}

bool MysqlCreateIndexService::isUpValid(const Commands::ConstCommandPtr &command
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

bool MysqlCreateIndexService::down(const Commands::ConstCommandPtr &command
                                   , CommandExecution::CommandExecutionContext &context) const
{
    QSharedPointer<const Commands::CreateIndex> createIndex(command.staticCast<const Commands::CreateIndex>());
    MysqlDropIndexService dropIndexService;
    return dropIndexService.up(Commands::CommandPtr(new Commands::DropIndex(createIndex->name())), context);
}

bool MysqlCreateIndexService::isDownValid(const Commands::ConstCommandPtr &command
                                          , const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(command);
    Q_UNUSED(context);
    return true;
}

} // namespace CommandExecution
