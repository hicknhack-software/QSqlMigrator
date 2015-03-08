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
#include "StandardCreateTable.h"

#include "SqlDatabaseAdapter/ColumnMapper.h"
#include "SqlDatabaseAdapter/Quotation.h"

#include "SqlSchemaCommand/CreateTable.h"
#include "SqlSchemaCommand/DropTable.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const QString &StandardCreateTable::commandType() const
{
    return SqlSchemaCommand::CreateTable::commandName();
}

bool
StandardCreateTable::execute(const SqlSchemaCommand::CreateTable &createTable, const Context &context)
{
    const auto columnMapper = context.sqlAdapter().columnMapper();
    Q_ASSERT(columnMapper);
    const auto quotation = context.sqlAdapter().quotation();
    Q_ASSERT(quotation);
    const auto createQuery = QString("CREATE TABLE %1 (%2)")
                                 .arg(quotation->quoteTableName(createTable.table().name()))
                                 .arg(columnMapper->generateColumnsDefinitionSql(createTable.table().columns()));

    return SqlDatabaseSchemaAdapter::CommandExecutor::executeQuery(createQuery, context);
}

bool
StandardCreateTable::execute(const Command &command, Context &context) const
{
    auto createTable = command.staticCast<const SqlSchemaCommand::CreateTable>();
    Q_ASSERT(createTable);

    bool success = execute(*createTable, context);

    if (success && context.undo().isUsed()) {
        context.undo().setCommand(command->reverse());
    }

    return success;
}

bool
StandardCreateTable::isValid(const Command &command, const Context &context) const
{
    auto createTable = command.staticCast<const SqlSchemaCommand::CreateTable>();
    Q_ASSERT(createTable);

    auto scope = context.logging()->scope([&]() { return QString("Standard%1").arg(createTable->toString()); });
    Q_UNUSED(scope);

    if (context.database().tables().contains(createTable->table().name())) {
        context.logging()->error("table already exists");
        return false;
    }

    return true;
}

} // namespace BaseSqlCommandService
} // namespace QSqlMigrator
