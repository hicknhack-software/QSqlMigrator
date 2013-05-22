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
#include "DbTable.h"

#include <QDebug>

namespace Scheme {

DbTable::DbTable(const QString &name)
    : m_name(name)
{
}

DbTable::DbTable(const QString &name, const QList<DbColumn> &columns)
    : m_name(name)
    , m_columns(columns)
{
}

QList<DbColumn> &DbTable::columns()
{
    return const_cast<QList<DbColumn> &>(static_cast<const DbTable &>(*this).columns());
}

const QList<DbColumn> &DbTable::columns() const
{
    return m_columns;
}

const QString &DbTable::name() const
{
    return m_name;
}

void DbTable::setName(const QString &newName)
{
    m_name = newName;
}

DbTable &DbTable::add(const DbColumn &newColumn)
{
    foreach(const DbColumn &column, m_columns) {
        if(column.name() == newColumn.name()) {
            ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "Table already contains column" << newColumn.name();
            return (*this);
        }
    }

    m_columns << newColumn;
    return (*this);
}

DbTable &DbTable::removeColumn(const QString &columnName)
{
    QMutableListIterator<DbColumn> columnIterator(m_columns);
    while(columnIterator.hasNext()) {
        columnIterator.next();
        const DbColumn &column = columnIterator.value();
        if(column.name() == columnName) {
            columnIterator.remove();
            break;
        }
    }

    return (*this);
}

} // namespace Scheme
