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

#include "config.h"

#include <QHash>
#include <QString>
#include <QSharedPointer>

namespace QSqlMigrator {
namespace LoggingTrace {
class Logging;
} // namespace LoggingTrace

namespace SqlSchema {

/// \brief sorting order
enum class SortOrder { Default = 0, Ascending = Default, Descending = 1 };

/// \brief inline value object for column references of indices
class QSQLMIGRATOR_DLL_EXPORT IndexColumn {
public:
    using Name = QString;
    using SortOrder = QSqlMigrator::SqlSchema::SortOrder;

public:
    IndexColumn(Name, SortOrder = SortOrder::Default);

    /// \return an global static reference to an invalid index column
    static const IndexColumn &invalid() Q_DECL_NOEXCEPT;

    const Name &name() const Q_DECL_NOEXCEPT;
    const SortOrder &sortOrder() const Q_DECL_NOEXCEPT;

    /// \return true if name is not empty
    bool isValid() const Q_DECL_NOEXCEPT;

    /// \return true if names of both columns are equal
    bool operator==(const IndexColumn &other) const Q_DECL_NOEXCEPT;

private:
    Name m_name;
    SortOrder m_sortOrder;
};

/// \brief value object representing the structure of an sql index.
class QSQLMIGRATOR_DLL_EXPORT Index {
public:
    using Name = QString;
    using TableName = QString;
    using Columns = QList<IndexColumn>;
    using Reporter = QSharedPointer<LoggingTrace::Logging>;

public:
    explicit Index(Name, TableName, Columns);

    /// \return an global static reference to an invalid index
    static const Index &invalid() Q_DECL_NOEXCEPT;

    const Name &name() const Q_DECL_NOEXCEPT;
    const TableName &tableName() const Q_DECL_NOEXCEPT;
    const Columns &columns() const Q_DECL_NOEXCEPT;

    /// \return true if all attributes are not emptry and all columns are valid
    bool isValid() const Q_DECL_NOEXCEPT;

    /// check and report all errors to the reporter
    void reportErrors(const Reporter &) const;

private:
    Name m_name;
    TableName m_tableName;
    Columns m_columns;
};

/// \brief builder helper to generate and manipulate an index
class QSQLMIGRATOR_DLL_EXPORT IndexBuilder {
public:
    using Name = QString;
    using TableName = QString;
    using Columns = QList<IndexColumn>;

public:
    explicit IndexBuilder(Name, TableName = {}, Columns = {});
    explicit IndexBuilder(const Index &index);

    operator Index() const;

    void setName(const Name &name);
    void setTableName(const TableName &tableName);

    IndexBuilder &operator<<(const IndexColumn &column);

private:
    Name m_name;
    TableName m_tableName;
    Columns m_columns;
};

inline IndexColumn::IndexColumn(Name name, SortOrder sortOrder)
    : m_name(std::move(name))
    , m_sortOrder(sortOrder)
{
}

inline const IndexColumn::Name &
IndexColumn::name() const Q_DECL_NOEXCEPT
{
    return m_name;
}

inline const IndexColumn::SortOrder &
IndexColumn::sortOrder() const Q_DECL_NOEXCEPT
{
    return m_sortOrder;
}

inline bool
IndexColumn::isValid() const Q_DECL_NOEXCEPT
{
    return (!m_name.isEmpty());
}

inline bool IndexColumn::operator==(const IndexColumn &other) const Q_DECL_NOEXCEPT
{
    return other.m_name == m_name;
}

inline Index::Index(QString name, QString tableName, Columns columns)
    : m_name(std::move(name))
    , m_tableName(std::move(tableName))
    , m_columns(std::move(columns))
{
}

inline const Index::Name &
Index::name() const Q_DECL_NOEXCEPT
{
    return m_name;
}

inline const Index::TableName &
Index::tableName() const Q_DECL_NOEXCEPT
{
    return m_tableName;
}

inline const Index::Columns &
Index::columns() const Q_DECL_NOEXCEPT
{
    return m_columns;
}

inline IndexBuilder::IndexBuilder(Name name, TableName tableName, Columns columns)
    : m_name(std::move(name))
    , m_tableName(std::move(tableName))
    , m_columns(std::move(columns))
{
}

inline IndexBuilder::IndexBuilder(const Index &index)
    : m_name(index.name())
    , m_tableName(index.tableName())
    , m_columns(index.columns())
{
}

inline IndexBuilder::operator Index() const
{
    return Index(m_name, m_tableName, m_columns);
}

inline void
IndexBuilder::setName(const Name &name)
{
    m_name = name;
}

inline void
IndexBuilder::setTableName(const TableName &tableName)
{
    m_tableName = tableName;
}

inline IndexBuilder &IndexBuilder::operator<<(const IndexColumn &column)
{
    m_columns << column;
    return *this;
}

} // namespace SqlSchema
} // namespace QSqlMigrator
