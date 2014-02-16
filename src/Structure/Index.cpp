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

Structure::Index::Builder::Builder(const QString &name, const QString &tableName)
    : m_name(name)
    , m_tableName(tableName)
{
    if(tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Index" << name << "with empty table name!";
    }
}

Index::Builder &Index::Builder::operator<<(const Index::Column &column)
{
    if( std::any_of( m_columns.begin(), m_columns.end(), [&column](const Index::Column& other)->bool { return other.name() == column.name(); } ) ) {
        ::qWarning() << LOG_PREFIX << "Column is added twice to an index!" << m_name;
        return *this;
    }
    m_columns << column;
    return *this;
}

Index::Builder::operator Index() const
{
    if( m_tableName.isEmpty() ) {
        return Index(m_name, m_columns);
    }
    return Index(m_name, m_tableName, m_columns);
}

Index::Index(const QString &name, const QString &tableName, const ColumnList &columns)
    : m_name(name)
    , m_tableName(tableName)
    , m_columns(columns)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Index with empty name!" << tableName;
    }
    if(tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Index" << name << "with empty table name!";
    }
}

Index::Index(const QString &name, const Index::ColumnList &columns)
    : m_name(name)
    , m_columns(columns)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Index with empty name!";
    }
}

} //namespace Structure
