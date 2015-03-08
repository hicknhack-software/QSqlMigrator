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
#include "StandardColumnMapper.h"

#include "SqlDatabaseAdapter/ValueTypeMapper.h"

#include <QStringList>

namespace QSqlMigrator {
namespace SqlDatabaseAdapter {

StandardColumnMapper::StandardColumnMapper(const ValueTypeMapper &valueTypeMapper)
    : m_valueTypeMapper(valueTypeMapper)
{
}

StandardColumnMapper::Sql
StandardColumnMapper::buildColumnTypeSql(const Column &column) const
{
    return m_valueTypeMapper->map(column.type());
}

void
StandardColumnMapper::buildColumnOptionsSql(const Column &column, const SqlOutputFunction &addOption) const
{
    if (column.isPrimary()) {
        addOption("PRIMARY KEY");
    }
    else if (!column.isNullable()) {
        addOption("NOT NULL");
    }
    if (column.isUnique()) {
        addOption("UNIQUE");
    }
    if (column.hasDefaultValue()) {
        addOption(QString("DEFAULT (%1)").arg(column.defaultValue()));
    }
}

StandardColumnMapper::Sql
StandardColumnMapper::generateColumnDefinitionSql(const Column &column) const
{
    QStringList columnParts;
    columnParts << column.name() << buildColumnTypeSql(column);
    buildColumnOptionsSql(column, [&](const QString &option) { columnParts << option; });
    return columnParts.join(" ");
}

StandardColumnMapper::Sql
StandardColumnMapper::generateColumnsDefinitionSql(const Columns &columns) const
{
    QStringList sqlColumnDefinitions;
    for (const auto &column : columns) {
        sqlColumnDefinitions << generateColumnDefinitionSql(column);
    }
    return sqlColumnDefinitions.join(", ");
}

StandardColumnMapper::Sql
StandardColumnMapper::generateIndexColumnDefinitionSql(const IndexColumn &column) const
{
    QString sqlSortOrder;
    switch (column.sortOrder()) {
    case SqlSchema::SortOrder::Ascending:
        sqlSortOrder = "ASC";
        break;
    case SqlSchema::SortOrder::Descending:
        sqlSortOrder = "DESC";
        break;
    }

    return QString("%1 %2").arg(column.name(), sqlSortOrder);
}

StandardColumnMapper::Sql
StandardColumnMapper::generateIndexColumnsDefinitionSql(const IndexColumns &indexColumns) const
{
    QStringList sqlIndexColumnDefinitions;
    for (const auto &indexColumn : indexColumns) {
        sqlIndexColumnDefinitions << generateIndexColumnDefinitionSql(indexColumn);
    }
    return sqlIndexColumnDefinitions.join(", ");
}

} // namespace Helper
} // namespace QSqlMigrator
