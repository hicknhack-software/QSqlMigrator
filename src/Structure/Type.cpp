#include "Type.h"

namespace Structure {

Type::Type(Type::Base base, quint64 precision, int scale)
    : m_base(base)
    , m_precision(precision)
    , m_scale(scale)
{
}

Type::Type(const QString &string)
    : m_base(Invalid)
    , m_precision(0)
    , m_scale(0)
    , m_string(string)
{
}

bool Type::isBaseType() const
{
    return m_base != Invalid;
}

bool Type::isString() const
{
    return m_base == Invalid && (!m_string.isEmpty());
}

bool Type::isValid() const
{
    return m_base != Invalid || (!m_string.isEmpty());
}

const QString &Type::string() const
{
    return m_string;
}

Type::Base Type::base() const {
    return m_base;
}

quint64 Type::precision(quint64 defaultValue) const {
    if( 0 == m_precision )
        return defaultValue;
    return m_precision;
}

int Type::scale() const {
    return m_scale;
}

const Type &Type::invalid()
{
    static Type invalid_sql_type(Type::Invalid);
    return invalid_sql_type;
}

} // namespace Structure
