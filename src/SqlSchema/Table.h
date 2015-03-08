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
#pragma once

#include "SqlSchema/Column.h"

#include "config.h"

#include <QString>
#include <QList>
#include <QStringList>
#include <QSharedPointer>

namespace QSqlMigrator {
namespace LoggingTrace {
class Logging;
} // namespace LoggingTrace

namespace SqlSchema {

/// \brief value object desribing the sql-table-structure
/// the order of the columns is important and therefore carefully preserved.
class QSQLMIGRATOR_DLL_EXPORT Table
{
public:
    using Name = QString;
    using Column = QSqlMigrator::SqlSchema::Column;
    using Columns = QList<Column>;
    using Reporter = QSharedPointer<LoggingTrace::Logging>;

public:
    explicit Table(Name, Columns);

    /// \return an global static reference to an invalid table
    static const Table &invalid() Q_DECL_NOEXCEPT;

    const Name &name() const Q_DECL_NOEXCEPT;
    const Columns &columns() const Q_DECL_NOEXCEPT;

    /// \return true if table name and columns are not empty and all columns are valid
    /// \note duplicate column names are not checked!
    bool isValid() const Q_DECL_NOEXCEPT;

    /// check and report all errors to the reporter
    void reportErrors(const Reporter &) const;

    /// \return true if a column with the given name exists
    bool hasColumn(const QString &columnName) const Q_DECL_NOEXCEPT;

    /// \return generated list of all column names
    QStringList columnNames() const;

    /// \return the column with the given name (the invalid column if not found)
    const Column &column(const QString &name) const Q_DECL_NOEXCEPT;

private:
    Name m_name;
    Columns m_columns;
};

/// \brief builder helper to generate and manipulate a table
class QSQLMIGRATOR_DLL_EXPORT TableBuilder {
public:
    using Name = QString;
    using Column = QSqlMigrator::SqlSchema::Column;
    using ColumnName = Column::Name;
    using Columns = QList<Column>;

public:
    explicit TableBuilder(Name = {}, Columns = {});
    explicit TableBuilder(const Table &);

    operator Table() const;

    /// change the table name
    void setName(const Name &);

    /// \return true if the column with the given name was removed
    bool dropColumn(const ColumnName &);

    /// \return true if the column with the given name was changed
    bool alterColumn(const ColumnName &, const Column &);

    /// append a column operator
    TableBuilder &operator<<(const Column &);

private:
    QString m_name;
    Columns m_columns;
};

inline Table::Table(Name name, Columns columns)
    : m_name(std::move(name))
    , m_columns(std::move(columns))
{
}

inline const Table::Name &
Table::name() const Q_DECL_NOEXCEPT
{
    return m_name;
}

inline const Table::Columns &
Table::columns() const Q_DECL_NOEXCEPT
{
    return m_columns;
}

inline TableBuilder::TableBuilder(Name name, Columns columns)
    : m_name(std::move(name))
    , m_columns(std::move(columns))
{
}

inline TableBuilder::TableBuilder(const Table &table)
    : m_name(table.name())
    , m_columns(table.columns())
{
}

inline TableBuilder::operator Table() const
{
    return Table(m_name, m_columns);
}

inline void
TableBuilder::setName(const Name &name)
{
    m_name = name;
}

inline TableBuilder &TableBuilder::operator<<(const Column &column)
{
    m_columns << column;
    return *this;
}

} // namespace SqlSchema
} // namespace QSqlMigrator
