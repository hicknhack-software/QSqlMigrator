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
#pragma once

#include "SqlSchema/Table.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutorContext.h"

namespace SqliteMigrator {
namespace SqlDatabaseSchemaAdapter {

// used only internally in other services, because SQLite can't alter columns
class SqliteAlterColumn
{
    using Table = QSqlMigrator::SqlSchema::Table;
    using Context = QSqlMigrator::SqlDatabaseSchemaAdapter::CommandExecutorContext;

public:
    static bool execute(const Table &before, const Table &after, Context&);
};

} // namespace SqlDatabaseSchemaAdapter
} // namespace SqliteMigrator
