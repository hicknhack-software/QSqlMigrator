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
#ifndef STRUCTURE_TABLE_H
#define STRUCTURE_TABLE_H

#include "Structure/Column.h"

#include <QList>
#include <QString>

namespace Structure {

typedef QList<Column> ColumnList;

/*!
 * \brief The Table class represents basic table-structure.
 */
class QSQLMIGRATOR_DLL_EXPORT Table
{
public:
    class QSQLMIGRATOR_DLL_EXPORT Builder
    {
    public:
        explicit Builder(const QString &name, const ColumnList& columns = ColumnList());
        Builder& operator <<(const Column& column);
        operator Table();

    private:
        QString m_name;
        ColumnList m_columns;
    };

    Table(const QString &name, const ColumnList &columns);

    const QString &name() const;
    const ColumnList &columns() const;

    bool isValid() const;

    bool hasColumn(const QString &columnName) const;

    //! \return a generated list of all column names
    QStringList columnNames() const;

    //! \return the cloned table without the specified column
    Table cloneWithoutColumn(const QString &columnName) const;

    //! \return the column with the given name
    Column fetchColumnByName(const QString &name, bool &success) const;

private:
    const QString m_name;
    const ColumnList m_columns;
};

inline Table::Builder::Builder(const QString &name, const ColumnList &columns)
    : m_name(name)
    , m_columns(columns)
{}

inline Table::Builder::operator Table()
{
    return Table(m_name, m_columns);
}

inline const QString &Table::name() const
{
    return m_name;
}

inline const ColumnList &Table::columns() const
{
    return m_columns;
}

inline bool Table::isValid() const
{
    return (!m_name.isEmpty()) && (!m_columns.isEmpty());
}

} //namespace Structure

#endif // STRUCTURE_TABLE_H
