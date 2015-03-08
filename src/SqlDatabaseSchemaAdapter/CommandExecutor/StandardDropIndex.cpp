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
#include "StandardDropIndex.h"

#include "SqlSchemaCommand/DropIndex.h"

#include "SqlDatabaseAdapter/Quotation.h"
#include "SqlDatabaseAdapter/SchemaReflection.h"

#include <QDebug>

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const StandardDropIndex::CommandType &
StandardDropIndex::commandType() const
{
    return SqlSchemaCommand::DropIndex::commandName();
}

bool
StandardDropIndex::execute(const SqlSchemaCommand::DropIndex &dropIndex, const Context &context)
{
    const auto quotation = context.sqlAdapter().quotation();
    Q_ASSERT(quotation);
    const auto sqlQuery = QString("DROP INDEX %1").arg(quotation->quoteIndexName(dropIndex.name()));

    return executeQuery(sqlQuery, context);
}

bool
StandardDropIndex::execute(const Command &command, Context &context) const
{
    auto dropIndex = command.staticCast<const SqlSchemaCommand::DropIndex>();
    Q_ASSERT(dropIndex);

    if (context.undo().isUsed() && !dropIndex->index().tableName().isEmpty()) {
        const auto schemaReflection = context.sqlAdapter().schemaReflection();
        Q_ASSERT(schemaReflection);

        const auto originalIndex =
            schemaReflection->getIndexDefinition(dropIndex->name(), dropIndex->index().tableName(), context.database());

        bool success = execute(*dropIndex, context);
        if (success && context.undo().isUsed() && originalIndex.isValid()) {
            auto undoCommand = QSharedPointer<SqlSchemaCommand::DropIndex>::create(originalIndex);
            context.undo().setCommand(undoCommand);
        }
        return success;
    }
    // without tableName no reflection is possible
    return execute(*dropIndex, context);
}

bool
StandardDropIndex::isValid(const Command &command, const Context &context) const
{
    auto dropIndex = command.staticCast<const SqlSchemaCommand::DropIndex>();
    Q_ASSERT(dropIndex);

    if (!dropIndex->index().tableName().isEmpty()) {
        auto scope = context.logging()->scope([&]() { return QString("Standard%1").arg(dropIndex->toString()); });
        Q_UNUSED(scope);

        const auto schemaReflection = context.sqlAdapter().schemaReflection();
        Q_ASSERT(schemaReflection);

        const auto index =
            schemaReflection->getIndexDefinition(dropIndex->name(), dropIndex->index().tableName(), context.database());
        if (!index.isValid()) {
            context.logging()->error("index does not exist");
            return false;
        }
    }
    return true;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
