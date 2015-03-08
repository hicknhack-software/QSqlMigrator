#pragma once

#include "config.h"

#include <QString>

namespace QSqlMigrator {
namespace SqlSchema {

/// \brief Standard Sql Types
enum class Type {
    Invalid,
    Char, ///< precision is length (defaults to 1)
    VarChar, ///< precision is maximum length (defaults to 255)
    String = VarChar,
    Binary, ///< precision represents length in bytes (required)
    VarBinary, ///< precision represents maximum bytes
    ByteArray = VarBinary, ///< Alias vor VarBinary
    Boolean,
    SmallInt, ///< precision is 5
    Integer, ///< precision defaults to 10
    BigInt, ///< precision is 19
    Decimal, ///< precision and scale required (total digits, digits after comma)
    Numeric = Decimal, ///< Alias for Decimal
    Float,
    DoublePrecision,
    Double = DoublePrecision, ///< Alias for Double Precision
    Date,
    Time,
    Timestamp,
    DateTime = Timestamp, ///< Alias for Timestamp
    Interval,
    Array,
    Multiset,
    Xml,
    Custom = 100, ///< Custom Types start here (your database adapter has to be able to convert this)
};

/// \brief inline value object that represents a sql type
class QSQLMIGRATOR_DLL_EXPORT ValueType
{
public:
    using Base = Type;
    using SqlString = QString;
    using Precision = quint64;
    using Scale = int;

public:
    /// \param string the string representation of an sql type
    explicit ValueType(SqlString);

    /// \param base the required base type
    /// \param precision if supported or required the precision or length
    /// \param scale if supported or required by the base type
    explicit ValueType(Base, Precision = 0, Scale = 0) Q_DECL_NOEXCEPT;

    /// \return an global static reference to an invalid type
    static const ValueType &invalid() Q_DECL_NOEXCEPT;

    /// \return true, if the string based type is used
    bool isString() const  Q_DECL_NOEXCEPT;

    /// \return true, if the base type is used
    bool isBaseType() const  Q_DECL_NOEXCEPT;

    /// \return the string representation of the sql type (if string form is used)
    const SqlString &string() const  Q_DECL_NOEXCEPT;

    /// \return the base type (if base form is used)
    Base base() const  Q_DECL_NOEXCEPT;

    /// \return the precision (only used for supported types - numeric and string)
    quint64 precision(Precision defaultValue = 0) const  Q_DECL_NOEXCEPT;

    /// \return the scale of the type (only used for numeric values)
    int scale() const  Q_DECL_NOEXCEPT;

    /// \return true if this type is not invalid
    bool isValid() const  Q_DECL_NOEXCEPT;

private:
    Base m_base;
    Precision m_precision;
    Scale m_scale;
    SqlString m_string;
};

inline ValueType::ValueType(SqlString string)
    : m_base(Type::Invalid)
    , m_precision(0)
    , m_scale(0)
    , m_string(std::move(string))
{
}

inline ValueType::ValueType(Base base, Precision precision, Scale scale) Q_DECL_NOEXCEPT
    : m_base(base)
    , m_precision(precision)
    , m_scale(scale)
{
}

inline bool
ValueType::isString() const Q_DECL_NOEXCEPT
{
    return (m_base == Type::Invalid) && (!m_string.isEmpty());
}

inline bool
ValueType::isBaseType() const Q_DECL_NOEXCEPT
{
    return (m_base != Type::Invalid);
}

inline const ValueType::SqlString &
ValueType::string() const Q_DECL_NOEXCEPT
{
    return m_string;
}

inline ValueType::Base
ValueType::base() const Q_DECL_NOEXCEPT
{
    return m_base;
}

inline ValueType::Precision
ValueType::precision(Precision defaultValue) const Q_DECL_NOEXCEPT
{
    if (0 == m_precision)
        return defaultValue;
    return m_precision;
}

inline ValueType::Scale
ValueType::scale() const Q_DECL_NOEXCEPT
{
    return m_scale;
}

inline bool
ValueType::isValid() const Q_DECL_NOEXCEPT
{
    return (m_base != Type::Invalid) || (!m_string.isEmpty());
}

} // namespace SqlSchema
} // namespace QSqlMigrator
