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
#include "SqliteAlterColumn.h"

#include "SqlSchemaCommand/DropTable.h"
#include "SqlSchemaCommand/CreateTable.h"
#include "SqlSchemaCommand/RenameTable.h"

#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardCreateTable.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardDropTable.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardRenameTable.h"

#include "SqlDatabaseAdapter/Quotation.h"

#include <QStringList>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

namespace SqliteMigrator {
namespace SqlDatabaseSchemaAdapter {

bool
SqliteAlterColumn::execute(const Table &origTable, const Table &newTable, Context &context)
{
    const auto tempTableName =
        QString("%1%2").arg(context.config().temporaryTablePrefix).arg(origTable.name());

    using namespace QSqlMigrator::SqlDatabaseSchemaAdapter;
    using namespace QSqlMigrator::SqlSchemaCommand;

    bool success =
        StandardRenameTable::execute(RenameTable(origTable.name(), tempTableName), context);
    if (!success)
        return false;

    success = StandardCreateTable::execute(CreateTable(newTable), context);
    if (!success)
        return false;

    const auto copyQuery =
        QString("INSERT INTO %1 SELECT * FROM %2")
            .arg(context.sqlAdapter().quotation()->quoteTableName(newTable.name()))
            .arg(context.sqlAdapter().quotation()->quoteTableName(tempTableName));

    ::qDebug() << "complete query-string looks like:";
    ::qDebug() << copyQuery;
    QSqlQuery query = context.database().exec(copyQuery);
    QSqlError error = query.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
        return false;
    }

    success = StandardDropTable::execute(DropTable(tempTableName), context);
    return success;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace SqliteMigrator
