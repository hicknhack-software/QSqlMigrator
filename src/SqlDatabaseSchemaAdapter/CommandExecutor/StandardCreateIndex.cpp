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
#include "StandardCreateIndex.h"

#include "SqlDatabaseAdapter/ColumnMapper.h"
#include "SqlDatabaseAdapter/Quotation.h"
#include "SqlDatabaseAdapter/SchemaReflection.h"

#include "SqlSchemaCommand/CreateIndex.h"
#include "SqlSchemaCommand/DropIndex.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const StandardCreateIndex::CommandType &
StandardCreateIndex::commandType() const
{
    return SqlSchemaCommand::CreateIndex::commandName();
}

bool
StandardCreateIndex::execute(const SqlSchemaCommand::CreateIndex &createIndex, const Context &context)
{
    const auto columnMapper = context.sqlAdapter().columnMapper();
    Q_ASSERT(columnMapper);
    const auto quotation = context.sqlAdapter().quotation();
    Q_ASSERT(quotation);
    const auto createQuery = QString("CREATE INDEX %1 ON %2 (%3)")
                                 .arg(quotation->quoteIndexName(createIndex.index().name()))
                                 .arg(quotation->quoteTableName(createIndex.index().tableName()))
                                 .arg(columnMapper->generateIndexColumnsDefinitionSql(createIndex.index().columns()));

    return executeQuery(createQuery, context);
}

bool
StandardCreateIndex::execute(const Command &command, Context &context) const
{
    auto createIndex = command.staticCast<const SqlSchemaCommand::CreateIndex>();
    Q_ASSERT(createIndex);

    bool success = execute(*createIndex, context);

    if (success && context.undo().isUsed())
        context.undo().setCommand(command->reverse());
    return success;
}

bool
StandardCreateIndex::isValid(const Command &command, const Context &context) const
{
    auto createIndex = command.staticCast<const SqlSchemaCommand::CreateIndex>();
    Q_ASSERT(createIndex);

    auto scope = context.logging()->scope([&]() { return QString("Standard%1").arg(createIndex->toString()); });
    Q_UNUSED(scope);

    const auto schemaReflection = context.sqlAdapter().schemaReflection();
    const auto index = schemaReflection->getIndexDefinition(createIndex->index().name(),
                                                            createIndex->index().tableName(), context.database());
    if (index.isValid()) {
        context.logging()->error("index already exists");
        return false;
    }
    if (!context.database().tables().contains(createIndex->index().tableName())) {
        context.logging()->error("table does not exist");
        return false;
    }
    return true;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
