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
#include "BaseSqlTypeMapperService.h"

#include <QDebug>

namespace Helper {

BaseSqlTypeMapperService::BaseSqlTypeMapperService()
{
    using namespace Structure;

    m_typeMap.insert(Type::Char,            "CHAR(%1)");
    m_typeMap.insert(Type::VarChar,         "VARCHAR(%1)");
    m_typeMap.insert(Type::Binary,          "BINARY(%1)");
    m_typeMap.insert(Type::VarBinary,       "VARBINARY(%1)");
    m_typeMap.insert(Type::Boolean,         "BOOLEAN");
    m_typeMap.insert(Type::SmallInt,        "SMALLINT");
    m_typeMap.insert(Type::Integer,         "INTEGER(%1)");
    m_typeMap.insert(Type::BigInt,          "BIGINT");
    m_typeMap.insert(Type::Decimal,         "DECIMAL(%1,%2)");
    m_typeMap.insert(Type::Float,           "FLOAT");
    m_typeMap.insert(Type::DoublePrecision, "DOUBLE PRECISION");
    m_typeMap.insert(Type::Date,            "DATE");
    m_typeMap.insert(Type::Time,            "TIME");
    m_typeMap.insert(Type::Timestamp,       "TIMESTAMP");
    m_typeMap.insert(Type::Interval,        "INTERVAL");
    //    typeMap.insert(Type::Array,           "ARRAY(%1)");
    //    typeMap.insert(Type::Multiset,        "MULTISET");
    //    typeMap.insert(Type::Xml,             "XML");
}

QString BaseSqlTypeMapperService::map(const Structure::Type &type) const
{
    if( type.isString() )
        return type.string();

    using namespace Structure;

    switch (type.base())
    {
    case Type::Char:
        return m_typeMap[Type::Char].arg(type.precision(1));

    case Type::Integer:
        return m_typeMap[Type::Integer].arg(type.precision(10));

    case Type::Decimal:
        return m_typeMap[Type::Decimal].arg(type.precision()).arg(type.scale());

    case Type::VarChar:
    case Type::Binary:
    case Type::VarBinary:
        return m_typeMap[type.base()].arg(type.precision());

    default:
        if (!m_typeMap.contains(type.base()))
            ::qWarning() << "unknown type";
        return m_typeMap[type.base()];
    }
}

} // namespace Helper
