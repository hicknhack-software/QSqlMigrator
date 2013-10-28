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
#ifndef STRUCTURE_INDEX_H
#define STRUCTURE_INDEX_H

#include "config.h"

#include <QHash>
#include <QString>

namespace Structure {

/*!
 * \brief The Index class represents a basic index-structure.
 */
class QSQLMIGRATOR_DLL_EXPORT Index
{
public:
    enum SortOrder{
        Default = 0,
        Ascending = 0, // ASC is default
        Descending = 1
    };

    class Column {
    private:
        QString m_name;
        SortOrder m_sortOrder;
    public:
        Column() {}
        Column(const QString& name, const SortOrder& sortOrder = Default) : m_name(name), m_sortOrder(sortOrder) {}
        const QString &name() const { return m_name; }
        const SortOrder &sortOrder() const { return m_sortOrder; }
        bool operator==(const Column& other) { return other.name() == m_name && other.sortOrder() == m_sortOrder; }
    };

    typedef QList<Column> ColumnList;

    explicit Index(const QString &name, const QString &tableName);
    explicit Index(const QString &name, const QString &tableName, const ColumnList &columns);

    const ColumnList &columns() const;
    const QString &name() const;
    const QString &tableName() const;

    //TODO: remove this method, sorting order of columns should be included on creation
    Index &addColumn(const QString &columnName, SortOrder sortOrder=Default);

private:
    QString m_name;
    QString m_tableName;
    ColumnList m_columns;
};

} //namespace Structure

#endif // STRUCTURE_INDEX_H
