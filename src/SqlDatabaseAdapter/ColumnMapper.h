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

#include "SqlSchema/Column.h"
#include "SqlSchema/Index.h"

#include "config.h"

#include <QString>

namespace QSqlMigrator {
namespace SqlDatabaseAdapter {

/// \brief interface for a service that can convert column and index definitions to proper SQL statements
class QSQLMIGRATOR_DLL_EXPORT ColumnMapper
{
public:
    using Sql = QString;
    using Column = SqlSchema::Column;
    using Columns = QList<Column>;
    using IndexColumn = SqlSchema::IndexColumn;
    using IndexColumns = QList<IndexColumn>;

public:
    virtual Sql generateColumnDefinitionSql(const Column &) const = 0;
    virtual Sql generateColumnsDefinitionSql(const Columns &) const = 0;

    virtual Sql generateIndexColumnDefinitionSql(const IndexColumn &) const = 0;
    virtual Sql generateIndexColumnsDefinitionSql(const IndexColumns &) const = 0;
};

} // namespace SqlDatabaseAdapter
} // namespace QSqlMigrator
