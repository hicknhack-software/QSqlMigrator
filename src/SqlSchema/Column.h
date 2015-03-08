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

#include "SqlSchema/ValueType.h"

#include "config.h"

#include <QString>
#include <QFlags>
#include <QSharedPointer>

namespace QSqlMigrator {
namespace LoggingTrace {
class Logging;
} // namespace LoggingTrace

namespace SqlSchema {

/// \brief inline value object representing the column-structure.
/// \note Columns are default NULL (as in SQLite, MySQL and PostgreSQL).
/// \note According to the SQL standard, PRIMARY KEY should always imply NOT NULL.
/// \note UNIQUE columns can contain several NULL values (as in SQLite, MySQL and PostgreSQL)
/// \note AutoIncrement is special: every DBMS handles some sort auf auto increment in it's own way.
class QSQLMIGRATOR_DLL_EXPORT Column
{
public:
    enum Attribute {
        None = 0,
        NotNullable = (1 << 0),
        Unique = (1 << 1),
        Primary = (1 << 2) | NotNullable | Unique,
        AutoIncrement = (1 << 3)
    };
    using Name = QString;
    using Type = QSqlMigrator::SqlSchema::ValueType;
    using DefaultValue = QString;
    using Attributes = QFlags<Attribute>;
    using Reporter = QSharedPointer<LoggingTrace::Logging>;

public:
    explicit Column(Name, Type, DefaultValue, Attributes = None);
    explicit Column(Name, Type, Attributes = None);

    /// \return an global static reference to an invalid column
    static const Column &invalid() Q_DECL_NOEXCEPT;

    const Name &name() const Q_DECL_NOEXCEPT;
    const Type &type() const Q_DECL_NOEXCEPT;
    const DefaultValue &defaultValue() const Q_DECL_NOEXCEPT;
    const Attributes &attributes() const Q_DECL_NOEXCEPT;

    // Check Attribute Flags
    bool isNullable() const Q_DECL_NOEXCEPT;
    bool isPrimary() const Q_DECL_NOEXCEPT;
    bool isUnique() const Q_DECL_NOEXCEPT;
    bool isAutoIncremented() const Q_DECL_NOEXCEPT;

    /// \return true if name of both columns are equal
    bool operator==(const Column &other) const Q_DECL_NOEXCEPT;

    /// \return true if a non-empty name and a valid type are present
    bool isValid() const Q_DECL_NOEXCEPT;

    /// check and report all errors to the reporter
    void reportErrors(const Reporter &) const;

    /// \return true if the defaultValue is not empty
    bool hasDefaultValue() const Q_DECL_NOEXCEPT;

private:
    Name m_name;
    Type m_type;
    DefaultValue m_defaultValue;
    Attributes m_attributes;
};

inline Column::Column(Name name, Type type, DefaultValue defaultValue, Attributes attributes)
    : m_name(std::move(name))
    , m_type(std::move(type))
    , m_defaultValue(std::move(defaultValue))
    , m_attributes(attributes)
{
}

inline Column::Column(Name name, Type type, Attributes attributes)
    : m_name(std::move(name))
    , m_type(std::move(type))
    , m_attributes(attributes)
{
}

inline const Column::Name &
Column::name() const Q_DECL_NOEXCEPT
{
    return m_name;
}

inline const Column::Type &
Column::type() const Q_DECL_NOEXCEPT
{
    return m_type;
}

inline const Column::DefaultValue &
Column::defaultValue() const Q_DECL_NOEXCEPT
{
    return m_defaultValue;
}

inline const Column::Attributes &
Column::attributes() const Q_DECL_NOEXCEPT
{
    return m_attributes;
}

inline bool
Column::isNullable() const Q_DECL_NOEXCEPT
{
    return !m_attributes.testFlag(NotNullable);
}

inline bool
Column::isPrimary() const Q_DECL_NOEXCEPT
{
    return m_attributes.testFlag(Primary);
}

inline bool
Column::isUnique() const Q_DECL_NOEXCEPT
{
    return m_attributes.testFlag(Unique);
}

inline bool
Column::isAutoIncremented() const Q_DECL_NOEXCEPT
{
    return m_attributes.testFlag(AutoIncrement);
}

inline bool Column::operator==(const Column &other) const Q_DECL_NOEXCEPT
{
    return m_name == other.m_name;
}

inline bool
Column::isValid() const Q_DECL_NOEXCEPT
{
    return (!m_name.isEmpty()) && m_type.isValid();
}

inline bool
Column::hasDefaultValue() const Q_DECL_NOEXCEPT
{
    return (!m_defaultValue.isEmpty());
}

} // namespace SqlSchema
} // namespace QSqlMigrator

Q_DECLARE_OPERATORS_FOR_FLAGS(QSqlMigrator::SqlSchema::Column::Attributes)
