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
 * \brief value object representing the structure of an sql index.
 */
class QSQLMIGRATOR_DLL_EXPORT Index
{
public:
    /*!
     * \brief index sorting order
     */
    enum SortOrder{
        Default = 0,
        Ascending = 0, // ASC is default
        Descending = 1
    };

    /*!
     * \brief index column reference
     */
    class QSQLMIGRATOR_DLL_EXPORT Column
    {
    public:
        Column(const QString& name, const SortOrder& sortOrder = Default);

        const QString &name() const;
        const SortOrder &sortOrder() const;

        bool operator==(const Column& other) const;

    private:
        QString m_name;
        SortOrder m_sortOrder;
    };

    typedef QList<Column> ColumnList;

    /*!
     * \brief helper to build Index classes
     */
    class QSQLMIGRATOR_DLL_EXPORT Builder
    {
    public:
        Builder(const QString &name, const QString &tableName);
        Builder(const QString &name);
        Builder& operator<<(const Index::Column& column);
        operator Index() const;

    private:
        QString m_name;
        QString m_tableName;
        ColumnList m_columns;
    };

    explicit Index(const QString &name, const QString &tableName, const ColumnList &columns);
    explicit Index(const QString &name, const ColumnList &columns);

    const QString &name() const;
    const QString &tableName() const;
    const ColumnList &columns() const;

    bool isValid() const;

private:
    QString m_name;
    QString m_tableName;
    ColumnList m_columns;
};

inline Index::Column::Column(const QString &name, const Index::SortOrder &sortOrder)
    : m_name(name)
    , m_sortOrder(sortOrder)
{}

inline const QString &Index::Column::name() const
{
    return m_name;
}

inline const Index::SortOrder &Index::Column::sortOrder() const
{
    return m_sortOrder;
}

inline bool Index::Column::operator==(const Index::Column &other) const
{
    return other.name() == m_name;
}

inline Index::Builder::Builder(const QString &name)
    : m_name(name)
{
}

inline const QString &Index::name() const
{
    return m_name;
}

inline const QString &Index::tableName() const
{
    return m_tableName;
}

inline const Index::ColumnList &Index::columns() const
{
    return m_columns;
}

inline bool Index::isValid() const
{
    return (!m_name.isEmpty()) && (!m_columns.isEmpty());
}

} //namespace Structure

#endif // STRUCTURE_INDEX_H
