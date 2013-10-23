#include "SqlType.h"

SqlType::SqlType() : m_type(QVariant::Invalid), m_precision(0), m_scale(0)
{
}

SqlType::SqlType(QVariant::Type t, int precision, int scale) : m_type(t), m_precision(precision), m_scale(scale)
{
}

QVariant::Type SqlType::type() const {
    return m_type;
}

int SqlType::precision() const {
    return m_precision;
}

int SqlType::scale() const {
    return m_scale;
}
