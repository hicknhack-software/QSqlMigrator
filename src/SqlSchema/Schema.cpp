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
#include "Schema.h"

#include "LoggingTrace/Logging.h"

#include <QStringList>

namespace QSqlMigrator {
namespace SqlSchema {

bool
Schema::hasTable(const TableName &name) const Q_DECL_NOEXCEPT
{
    return std::any_of(m_tables.begin(), m_tables.end(), [&](const Table &table) { return table.name() == name; });
}

bool
Schema::hasIndex(const IndexName &name) const Q_DECL_NOEXCEPT
{
    return std::any_of(m_indices.begin(), m_indices.end(), [&](const Index &index) { return index.name() == name; });
}

const Table &
Schema::table(const TableName &name) const Q_DECL_NOEXCEPT
{
    auto it = std::find_if(m_tables.begin(), m_tables.end(), [&](const Table &table) { return table.name() == name; });
    if (it != m_tables.end()) {
        return *it;
    }
    return Table::invalid();
}

const Index &
Schema::index(const IndexName &name) const Q_DECL_NOEXCEPT
{
    auto it =
        std::find_if(m_indices.begin(), m_indices.end(), [&](const Index &index) { return index.name() == name; });
    if (it != m_indices.end()) {
        return *it;
    }
    return Index::invalid();
}

void
Schema::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return QStringLiteral("validating schema"); });
    Q_UNUSED(scope);

    QStringList tableNames;
    for (const auto &table : m_tables) {
        table.reportErrors(reporter);
        if (tableNames.contains(table.name()))
            reporter->error(QString("table '%1' occurs more than once").arg(table.name()));
        else
            tableNames << table.name();
    }

    QStringList indexNames;
    for (const auto &index : m_indices) {
        index.reportErrors(reporter);
        if (indexNames.contains(index.name()))
            reporter->error(QString("index '%1' occurs more than once").arg(index.name()));
        else
            indexNames << index.name();
    }
}

void
SchemaBuilder::createTable(const Table &table)
{
    m_tables << table;
}

bool
SchemaBuilder::alterTable(const TableName &tableName, const Table &table)
{
    auto it =
        std::find_if(m_tables.begin(), m_tables.end(), [&](const Table &table) { return table.name() == tableName; });
    if (it == m_tables.end())
        return false;
    *it = table;
    return true;
}

bool
SchemaBuilder::dropTable(const TableName &tableName)
{
    auto it =
        std::find_if(m_tables.begin(), m_tables.end(), [&](const Table &table) { return table.name() == tableName; });
    if (it == m_tables.end())
        return false;
    m_tables.erase(it);
    return true;
}

void
SchemaBuilder::createIndex(const Index &index)
{
    m_indices << index;
}

bool
SchemaBuilder::alterIndex(const IndexName &indexName, const Index &index)
{
    auto it =
        std::find_if(m_indices.begin(), m_indices.end(), [&](const Index &index) { return index.name() == indexName; });
    if (it == m_indices.end())
        return false;
    *it = index;
    return true;
}

bool
SchemaBuilder::dropIndex(const IndexName &indexName)
{
    auto it =
        std::find_if(m_indices.begin(), m_indices.end(), [&](const Index &index) { return index.name() == indexName; });
    if (it == m_indices.end())
        return false;
    m_indices.erase(it);
    return true;
}

} // namespace SqlSchema
} // namespace QSqlMigrator
