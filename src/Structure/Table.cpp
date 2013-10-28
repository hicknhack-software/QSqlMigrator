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

namespace Structure {

Table::Table()
{
}

Table::Table(const QString &name)
    : m_name(name)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Table with empty name!";
    }
}

Table::Table(const QString &name, const QList<Column> &columns)
    : m_name(name)
    , m_columns(columns)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Table with empty name!";
    }
}

const QString &Table::name() const
{
    return m_name;
}

const QList<Column> &Table::columns() const
{
    return m_columns;
}

const QString Table::joinedColumnNames(const QString &delimiter) const
{
    QStringList slColumnNames;
    foreach (const Column &column, m_columns) {
        slColumnNames << column.name();
    }
    return slColumnNames.join(delimiter);
}

const Table Table::copyWithoutColumn(const Table &table, const QString &columnName)
{
    QList<Column> newColumnList;
    newColumnList.reserve( ::qMax( 0, table.columns().size() - 1 ));
    foreach (const Column &column, table.columns()) {
        if (column.name() != columnName) {
            newColumnList << column;
        }
    }
    return Table(table.name(), newColumnList);
}

Table &Table::add(const Column &column)
{
    m_columns.append(column);
    return (*this);
}

Column Table::fetchColumnByName(const QString &name, bool &success)
{
    foreach (Structure::Column column, m_columns) {
        if (column.name() == name) {
            success = true;
            return column;
        }
    }
    success = false;
    return Column();
}

} //namespace Structure
