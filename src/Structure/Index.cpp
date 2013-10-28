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

#include <QDebug>
#include <QStringList>

namespace Structure {

Index::Index(const QString &name, const QString &tableName)
    : m_name(name)
    , m_tableName(tableName)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Index with empty name!";
    }

    if(tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Index" << m_name << "with empty table name!";
    }
}

Index::Index(const QString &name, const QString &tableName, const ColumnList &columns)
    : m_name(name)
    , m_tableName(tableName)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Index with empty name!";
    }

    if(tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Index" << m_name << "with empty table name!";
    }

    foreach (const Column &column, columns) {
        m_columns.append(column);
    }
}

const QString &Index::name() const
{
    return m_name;
}

const QString &Index::tableName() const
{
    return m_tableName;
}

const Index::ColumnList &Index::columns() const
{
    return m_columns;
}

Index &Index::addColumn(const QString &columnName, Index::SortOrder sortOrder)
{
    if(columnName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Can't add column with empty name to index" << m_name;
        return (*this);
    }

    foreach(Column column, m_columns) {
        if (column.name() == columnName) {
            ::qWarning() << LOG_PREFIX << "Column" << columnName << "already added to index" << m_name;
            return (*this);
        }
    }

    m_columns.append(Column(columnName, sortOrder));
    return (*this);
}

} //namespace Structure
