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
#include "PostgresqlTypeMapperService.h"

#include "Structure/Column.h"

#include <QDebug>

namespace Helper {

PostgresqlTypeMapperService::PostgresqlTypeMapperService()
{
    typeMap.insert(QVariant::Double,    "double precision");
    typeMap.insert(QVariant::Time,      "time without time zone");
    typeMap.insert(QVariant::DateTime,  "timestamp without time zone");
    typeMap.insert(QVariant::Char,      "character(%1)");
    typeMap.insert(QVariant::String,    "character varying(%1)");
    typeMap.insert(QVariant::ByteArray, "bytea");
}

} // namespace Helper
