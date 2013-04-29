#include "DbColumn.h"

namespace Scheme {

DbColumn::DbColumn(const QString &name, const QString &sqlType, Attributes attributes)
    : m_name(name)
    , m_sqlType(sqlType)
    , m_attributes(attributes)
{
}

QString &DbColumn::name()
{
    return const_cast<QString &>(static_cast<const DbColumn &>(*this).name());
}

const QString &DbColumn::name() const
{
    return m_name;
}

QString &DbColumn::sqlType()
{
    return const_cast<QString &>(static_cast<const DbColumn &>(*this).sqlType());
}

const QString &DbColumn::sqlType() const
{
    return m_sqlType;
}

DbColumn::Attributes &DbColumn::attributes()
{
    return const_cast<Attributes &>(static_cast<const DbColumn &>(*this).attributes());
}

const DbColumn::Attributes &DbColumn::attributes() const
{
    return m_attributes;
}

} // namespace Scheme
