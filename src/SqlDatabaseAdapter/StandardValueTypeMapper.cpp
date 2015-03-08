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
#include "StandardValueTypeMapper.h"

#include <QDebug>

namespace QSqlMigrator {
namespace SqlDatabaseAdapter {

StandardValueTypeMapper::StandardValueTypeMapper()
{
    using BaseType = Type::Base;
    m_typeMap.insert(BaseType::Char,            "CHAR(%1)");
    m_typeMap.insert(BaseType::VarChar,         "VARCHAR(%1)");
    m_typeMap.insert(BaseType::Binary,          "BINARY(%1)");
    m_typeMap.insert(BaseType::VarBinary,       "VARBINARY(%1)");
    m_typeMap.insert(BaseType::Boolean,         "BOOLEAN");
    m_typeMap.insert(BaseType::SmallInt,        "SMALLINT");
    m_typeMap.insert(BaseType::Integer,         "INTEGER(%1)");
    m_typeMap.insert(BaseType::BigInt,          "BIGINT");
    m_typeMap.insert(BaseType::Decimal,         "DECIMAL(%1,%2)");
    m_typeMap.insert(BaseType::Float,           "FLOAT");
    m_typeMap.insert(BaseType::DoublePrecision, "DOUBLE PRECISION");
    m_typeMap.insert(BaseType::Date,            "DATE");
    m_typeMap.insert(BaseType::Time,            "TIME");
    m_typeMap.insert(BaseType::Timestamp,       "TIMESTAMP");
    m_typeMap.insert(BaseType::Interval,        "INTERVAL");
    //    typeMap.insert(BaseType::Array,           "ARRAY(%1)");
    //    typeMap.insert(BaseType::Multiset,        "MULTISET");
    //    typeMap.insert(BaseType::Xml,             "XML");
}

StandardValueTypeMapper::Sql
StandardValueTypeMapper::map(const Type &type) const
{
    if (type.isString())
        return type.string();

    using BaseType = Type::Base;
    switch (type.base())
    {
    case BaseType::Char:
        return m_typeMap[BaseType::Char].arg(type.precision(1));

    case BaseType::Integer:
        return m_typeMap[BaseType::Integer].arg(type.precision(10));

    case BaseType::Decimal:
        return m_typeMap[BaseType::Decimal].arg(type.precision()).arg(type.scale());

    case BaseType::VarChar:
    case BaseType::Binary:
    case BaseType::VarBinary:
        return m_typeMap[type.base()].arg(type.precision());

    default:
        if (!m_typeMap.contains(type.base()))
            ::qWarning() << "unknown type";
        return m_typeMap[type.base()];
    }
}

} // namespace SqlDatabaseAdapter
} // namespace QSqlMigrator
