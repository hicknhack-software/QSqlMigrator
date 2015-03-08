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

#include "SqlSchema/Table.h"
#include "SqlSchema/Index.h"

#include "config.h"

#include <QList>
#include <QSharedPointer>

namespace QSqlMigrator {
namespace LoggingTrace {
class Logging;
} // namespace LoggingTrace

namespace SqlSchema {

/// \brief value object representing the sql schema
class QSQLMIGRATOR_DLL_EXPORT Schema {
public:
    using Table = QSqlMigrator::SqlSchema::Table;
    using Index = QSqlMigrator::SqlSchema::Index;
    using TableName = Table::Name;
    using IndexName = Index::Name;
    using Tables = QList<Table>;
    using Indices = QList<Index>;
    using Reporter = QSharedPointer<LoggingTrace::Logging>;

public:
    Schema(Tables, Indices);

    const Tables &tables() const Q_DECL_NOEXCEPT;
    const Indices &indices() const Q_DECL_NOEXCEPT;

    bool hasTable(const TableName &) const Q_DECL_NOEXCEPT;
    bool hasIndex(const IndexName &) const Q_DECL_NOEXCEPT;

    const Table &table(const TableName &) const Q_DECL_NOEXCEPT;
    const Index &index(const IndexName &) const Q_DECL_NOEXCEPT;

    /// check and report all errors to the reporter
    void reportErrors(const Reporter &) const;

private:
    Tables m_tables;
    Indices m_indices;
};

/// \brief builder helper to generate and manipulate sql schema
class QSQLMIGRATOR_DLL_EXPORT SchemaBuilder {
public:
    using Table = QSqlMigrator::SqlSchema::Table;
    using Index = QSqlMigrator::SqlSchema::Index;
    using TableName = Table::Name;
    using IndexName = Index::Name;
    using Tables = QList<Table>;
    using Indices = QList<Index>;

public:
    SchemaBuilder();
    explicit SchemaBuilder(const Schema &schema);

    operator Schema() const;

    void createTable(const Table &);
    bool alterTable(const TableName &, const Table &);
    bool dropTable(const TableName &);

    void createIndex(const Index &);
    bool alterIndex(const IndexName &, const Index &);
    bool dropIndex(const IndexName &);

private:
    Tables m_tables;
    Indices m_indices;
};

inline Schema::Schema(Tables tables, Indices indices)
    : m_tables(std::move(tables))
    , m_indices(std::move(indices))
{
}

inline const Schema::Tables &
Schema::tables() const Q_DECL_NOEXCEPT
{
    return m_tables;
}

inline const Schema::Indices &
Schema::indices() const Q_DECL_NOEXCEPT
{
    return m_indices;
}

inline SchemaBuilder::SchemaBuilder()
{
}

inline SchemaBuilder::SchemaBuilder(const Schema &schema)
    : m_tables(schema.tables())
    , m_indices(schema.indices())
{
}

inline SchemaBuilder::operator Schema() const
{
    return {m_tables, m_indices};
}

} // namespace SqlSchema
} // namespace QSqlMigrator
