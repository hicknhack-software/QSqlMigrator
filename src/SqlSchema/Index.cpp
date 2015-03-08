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
#include "Index.h"

#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlSchema {

const IndexColumn &
IndexColumn::invalid() Q_DECL_NOEXCEPT
{
    static IndexColumn s_invalid{{}};
    return s_invalid;
}

const Index &
Index::invalid() Q_DECL_NOEXCEPT
{
    static Index s_invalid{{}, {}, {}};
    return s_invalid;
}

bool
Index::isValid() const Q_DECL_NOEXCEPT
{
    if (m_name.isEmpty() || m_tableName.isEmpty() || m_columns.isEmpty())
        return false;
    if (std::all_of(m_columns.begin(), m_columns.end(),
                    [](const IndexColumn &column) -> bool { return column.isValid(); }))
        return false;
    return true;
}

void
Index::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return QString("index '%1' for table '%2'").arg(m_name).arg(m_tableName); });
    Q_UNUSED(scope);

    if (m_name.isEmpty())
        reporter->error("name of index is empty");
    if (m_tableName.isEmpty())
        reporter->error("name of table is empty");
    if (m_columns.isEmpty())
        reporter->error("no columns");

    QStringList names;
    for (const auto &column : m_columns) {
        if (column.name().isEmpty()) {
            reporter->error("name of column is empty");
            break;
        }
        if (names.contains(column.name())) {
            reporter->error(QString("column '%1' occurs more than once").arg(column.name()));
        }
        else {
            names << column.name();
        }
    }
}

} // namespace SqlSchema
} // namespace QSqlMigrator
