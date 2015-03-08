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
#include "SqlSchema/Index.h"

#include "config.h"

#include <QSqlDatabase>

namespace QSqlMigrator {
namespace SqlDatabaseAdapter {

/// \brief interface for a service that extracts existing sql schema details from a sql database
class QSQLMIGRATOR_DLL_EXPORT SchemaReflection
{
public:
    using Table = SqlSchema::Table;
    using Index = SqlSchema::Index;
    using TableName = Table::Name;
    using IndexName = Index::Name;
    using SqlDatabase = QSqlDatabase;

public:
    /// \return the definition of an existing table in the databse
    virtual Table getTableDefinition(const TableName &, SqlDatabase) const = 0;

    /// \return the definition of an existing index in the database for a table
    virtual Index getIndexDefinition(const IndexName &, const TableName &, SqlDatabase) const = 0;
};

} // namespace SqlDatabaseAdapter
} // namespace QSqlMigrator
