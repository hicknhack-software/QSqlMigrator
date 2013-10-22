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

#include "Structure/Column.h"

#include <QDebug>

namespace Helper {

BaseSqlTypeMapperService::BaseSqlTypeMapperService()
{
    typeMap.insert(QVariant::Bool,      "boolean");
    typeMap.insert(QVariant::Int,       "integer");
    typeMap.insert(QVariant::LongLong,  "bigint");
    typeMap.insert(QVariant::Double,    "double");
    typeMap.insert(QVariant::Date,      "date");
    typeMap.insert(QVariant::Time,      "time");
    typeMap.insert(QVariant::DateTime,  "datetime");
    typeMap.insert(QVariant::Char,      "char(%1)");
    typeMap.insert(QVariant::String,    "varchar(%1)");
    typeMap.insert(QVariant::ByteArray, "blob");
}

QString BaseSqlTypeMapperService::map(const sqlType &type) const
{
    QString sqlTypeString;

    switch (type.type) {
    case QVariant::Char:
        if (type.n)
            sqlTypeString = typeMap[QVariant::Char].arg(type.n);
        else
            sqlTypeString = typeMap[QVariant::Char].arg(1);
        break;
    case QVariant::String:
        sqlTypeString = typeMap[QVariant::String].arg(type.n);
        break;
    case QVariant::Double:
        if (type.n !=0 && type.m != 0)
            sqlTypeString = QString("numeric(%1,%2)").arg(QString::number(type.n), QString::number(type.m));
        else
            sqlTypeString = typeMap[QVariant::Double];
        break;
    default:
        if (!typeMap.contains(type.type))
            ::qWarning() << "unknown type";
        sqlTypeString = typeMap[type.type];
        break;
    }

    return sqlTypeString;
}

} // namespace Helper
