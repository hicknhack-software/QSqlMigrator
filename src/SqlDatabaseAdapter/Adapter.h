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

#include <QSqlDatabase>
#include <QSharedPointer>

namespace QSqlMigrator {
namespace SqlDatabaseAdapter {

class Quotation;
class ValueTypeMapper;
class ColumnMapper;
class SchemaReflection;

/// \brief inline repository of all sql adapters
class Adapter {
public:
    using Database = QSqlDatabase;
    using Quotation = QSharedPointer<const QSqlMigrator::SqlDatabaseAdapter::Quotation>;
    using ValueTypeMapper = QSharedPointer<const QSqlMigrator::SqlDatabaseAdapter::ValueTypeMapper>;
    using ColumnMapper = QSharedPointer<const QSqlMigrator::SqlDatabaseAdapter::ColumnMapper>;
    using SchemaReflection =
        QSharedPointer<const QSqlMigrator::SqlDatabaseAdapter::SchemaReflection>;

public:
    /// construct an invalid adapter
    Adapter() Q_DECL_NOEXCEPT;

    /// construct a valid adapter (every value is required)
    Adapter(Database, Quotation, ValueTypeMapper, ColumnMapper, SchemaReflection) Q_DECL_NOEXCEPT;

    /// \return if the adapter is valid
    bool isValid() const;

    Database database() const;
    const Quotation &quotation() const Q_DECL_NOEXCEPT;
    const ValueTypeMapper &valueTypeMapper() const Q_DECL_NOEXCEPT;
    const ColumnMapper &columnMapper() const Q_DECL_NOEXCEPT;
    const SchemaReflection &schemaReflection() const Q_DECL_NOEXCEPT;

private:
    Database m_database;
    Quotation m_quotation;
    ValueTypeMapper m_valueTypeMapper;
    ColumnMapper m_columnMapper;
    SchemaReflection m_schemaReflection;
};

inline Adapter::Adapter()
{
}

inline Adapter::Adapter(Database database,
                        Quotation quotation,
                        ValueTypeMapper valueTypeMapper,
                        ColumnMapper columnMapper,
                        SchemaReflection schemaReflection) Q_DECL_NOEXCEPT
    : m_database(std::move(database)),
      m_quotation(std::move(quotation)),
      m_valueTypeMapper(std::move(valueTypeMapper)),
      m_columnMapper(std::move(columnMapper)),
      m_schemaReflection(std::move(schemaReflection))
{
}

inline bool
Adapter::isValid() const
{
    return m_database.isValid() && m_quotation && m_valueTypeMapper && m_columnMapper &&
           m_schemaReflection;
}

inline Adapter::Database
Adapter::database() const
{
    return m_database;
}

inline const Adapter::Quotation &
Adapter::quotation() const Q_DECL_NOEXCEPT
{
    return m_quotation;
}

inline const Adapter::ValueTypeMapper &
Adapter::valueTypeMapper() const Q_DECL_NOEXCEPT
{
    return m_valueTypeMapper;
}

inline const Adapter::ColumnMapper &
Adapter::columnMapper() const Q_DECL_NOEXCEPT
{
    return m_columnMapper;
}

inline const Adapter::SchemaReflection &
Adapter::schemaReflection() const Q_DECL_NOEXCEPT
{
    return m_schemaReflection;
}

} // namespace SqlDatabaseAdapter
} // namespace QSqlMigrator
