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

#include <QDebug>
#include <QStringList>

#include <algorithm>

namespace Structure {

Table::Builder &Table::Builder::operator <<(const Column &column)
{
    if (std::any_of( m_columns.begin(), m_columns.end(), [&column](const Column& other)->bool { return other.name() == column.name(); })) {
        ::qWarning() << LOG_PREFIX << "Each table can only one column for each name" << m_name << column.name();
        return *this;
    }
    m_columns << column;
    return *this;
}

Table::Table(const QString &name, const ColumnList &columns)
    : m_name(name)
    , m_columns(columns)
{
    if (name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Table with empty name!";
    }
    if (std::any_of(columns.begin(), columns.end(), [](const Column& column)->bool { return !column.isValid(); })) {
        ::qWarning() << LOG_PREFIX << "Table with invalid columns!" << name;
    }
}

bool Table::hasColumn(const QString &columnName) const
{
    return std::any_of(m_columns.begin(), m_columns.end(), [&columnName](const Column& column)->bool { return column.name()==columnName; });
}

QStringList Table::columnNames() const
{
    QStringList columnNames;
    foreach (const Column &column, m_columns) {
        columnNames << column.name();
    }
    return columnNames;
}

Table Table::cloneWithoutColumn(const QString &columnName) const
{
    QList<Column> newColumnList;
    newColumnList.reserve( ::qMax( 0, columns().size() - 1 ));
    foreach (const Column &column, columns()) {
        if (column.name() != columnName) {
            newColumnList << column;
        }
    }
    return Table(name(), newColumnList);

}

Column Table::fetchColumnByName(const QString &name, bool &success) const
{
    foreach (Structure::Column column, m_columns) {
        if (column.name() == name) {
            success = true;
            return column;
        }
    }
    success = false;
    return Column(name, Type::invalid());
}

} //namespace Structure
