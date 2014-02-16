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
#include "MysqlMigrator/Helper/MysqlTypeMapperService.h"

#include "MigrationTracker/MigrationTrackerService.h"

#include <QDebug>

namespace Helper {

MysqlTypeMapperService::MysqlTypeMapperService()
{
    using namespace Structure;

    m_typeMap.insert(Type::Boolean, "TINYINT(1)");
    m_typeMap.insert(Type::Integer, "INT(%1)");
    m_typeMap.insert(Type::BigInt,  "BIGINT(%1)");
    m_typeMap.insert(Type::DoublePrecision,  "DOUBLE");
}

QString MysqlTypeMapperService::map(const Structure::Type &type) const
{
    using namespace Structure;

    switch (type.base())
    {
    case Type::BigInt:
        return m_typeMap[Type::BigInt].arg(type.precision(20));

    case Type::VarBinary:
    {
        const quint64 precision = type.precision();
        if (precision <= 255 && precision > 0)
            return "TINYBLOB";
        if (precision <= 65535)
            return "BLOB";
        if (precision <= 16777215)
            return "MEDIUMBLOB";
        if (precision <= 4294967295)
            return "LONGBLOB";
        ::qWarning() << "blob is too large";
        return "LONGBLOB";
    }
    case Type::VarChar:
    {
        const quint64 precision = type.precision();
        if (precision > 65535)
            return "TEXT";
        return BaseSqlTypeMapperService::map(type);
    }
    default:
        return BaseSqlTypeMapperService::map(type);
    }
}

} // namespace Helper
