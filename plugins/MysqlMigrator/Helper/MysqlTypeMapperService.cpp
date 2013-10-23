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

#include <QDebug>

namespace Helper {

MysqlTypeMapperService::MysqlTypeMapperService()
{
    typeMap.insert(QVariant::Bool,      "tinyint(1)");
    typeMap.insert(QVariant::Int,       "int(11)");
    typeMap.insert(QVariant::LongLong,  "bigint(20)");
    typeMap.insert(QVariant::ByteArray, "%1blob");
}

QString MysqlTypeMapperService::map(const SqlType &type) const
{
    QString sqlTypeString;

    switch (type.type()) {
    case QVariant::ByteArray:
        if (const int precision = type.precision()) {
            if (precision <= 255)
                sqlTypeString = typeMap[QVariant::ByteArray].arg("tiny");
            else if (precision <= 65535)
                sqlTypeString = typeMap[QVariant::ByteArray].arg("");
            else if (precision <= 16777215)
                sqlTypeString = typeMap[QVariant::ByteArray].arg("medium");
            else if (precision <= 4294967295)
                sqlTypeString = typeMap[QVariant::ByteArray].arg("long");
        }
        else
            sqlTypeString = typeMap[QVariant::ByteArray].arg("");
        break;
    case QVariant::Char:
        if (!type.precision())
            sqlTypeString = typeMap[QVariant::Char].arg(type.precision());
        else
            sqlTypeString = typeMap[QVariant::Char].arg(1);
        break;
    case QVariant::String:
        sqlTypeString = typeMap[QVariant::String].arg(type.precision());
        break;
    case QVariant::Double:
        if (type.precision() !=0 && type.scale() != 0)
            sqlTypeString = QString("decimal(%1,%2)").arg(QString::number(type.precision()), QString::number(type.scale()));
        else
            sqlTypeString = typeMap[QVariant::Double];
        break;
    default:
        if (!typeMap.contains(type.type()))
            ::qWarning() << "unknown type";
        sqlTypeString = typeMap[type.type()];
        break;
    }

    return sqlTypeString;
}

} // namespace Helper
