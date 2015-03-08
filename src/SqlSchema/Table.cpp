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
#include "Table.h"

#include "LoggingTrace/Logging.h"

#include <algorithm>

namespace QSqlMigrator {
namespace SqlSchema {

const Table &
Table::invalid() Q_DECL_NOEXCEPT
{
    static Table s_invalid{{}, {}};
    return s_invalid;
}

bool
Table::isValid() const Q_DECL_NOEXCEPT
{
    if (m_name.isEmpty() || m_columns.isEmpty())
        return false;
    if (std::all_of(m_columns.begin(), m_columns.end(), [](const Column &column) -> bool { return column.isValid(); }))
        return false;
    return true;
}

void
Table::reportErrors(const Table::Reporter &reporter) const
{
    if (m_name.isEmpty())
        reporter->error("name of table is empty");
    else {
        auto scope = reporter->scope([&]() { return QString("table '%1'").arg(m_name); });
        Q_UNUSED(scope);

        if (m_columns.isEmpty())
            reporter->error("no columns");
        bool hasPrimaryIndex = false;
        QStringList names;
        for (const auto &column : m_columns) {
            column.reportErrors(reporter);
            if (column.isPrimary())
                hasPrimaryIndex = true;
            if (names.contains(column.name()))
                reporter->error(QString("column '%1' occurs more than once").arg(column.name()));
            else
                names << column.name();
        }
        if (!hasPrimaryIndex)
            reporter->error("no primary index");
    }
}

bool
Table::hasColumn(const QString &columnName) const Q_DECL_NOEXCEPT
{
    return std::any_of(m_columns.begin(), m_columns.end(),
                       [&](const Column &column) -> bool { return column.name() == columnName; });
}

QStringList
Table::columnNames() const
{
    QStringList columnNames;
    foreach (auto &&column, m_columns) {
        columnNames << column.name();
    }
    return columnNames;
}

const Column &
Table::column(const QString &name) const Q_DECL_NOEXCEPT
{
    auto it =
        std::find_if(m_columns.begin(), m_columns.end(), [&](const Column &column) { return column.name() == name; });
    if (it != m_columns.end()) {
        return *it;
    }
    return Column::invalid();
}

bool
TableBuilder::dropColumn(const ColumnName &columnName)
{
    auto it = std::find_if(m_columns.begin(), m_columns.end(),
                           [&](const Column &column) { return column.name() == columnName; });
    if (it != m_columns.end()) {
        m_columns.erase(it);
        return true;
    }
    return false;
}

bool
TableBuilder::alterColumn(const ColumnName &columnName, const TableBuilder::Column &column)
{
    auto it = std::find_if(m_columns.begin(), m_columns.end(),
                           [&](const Column &column) { return column.name() == columnName; });
    if (it != m_columns.end()) {
        *it = column;
        return true;
    }
    return false;
}

} // namespace SqlSchema
} // namespace QSqlMigrator
