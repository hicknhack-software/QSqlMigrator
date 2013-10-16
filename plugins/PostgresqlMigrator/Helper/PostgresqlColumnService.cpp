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
#include "PostgresqlMigrator/Helper/PostgresqlColumnService.h"

#include <QStringList>

#include <QDebug>

using namespace Structure;

namespace Helper {

PostgresqlColumnService::PostgresqlColumnService()
{
}

QString PostgresqlColumnService::generateColumnDefinitionSql(const Column &column) const
{
    QStringList sqlColumnOptions;
    if (column.isPrimary()) {
        sqlColumnOptions << "PRIMARY KEY";
    }
    QString sqlType;
    bool serial = false;
    if (column.isAutoIncremented()) {
        // PostgreSQL has no auto increment, instead there are serials (as types)
        sqlType = "SERIAL"; // TODO: what do with SMALLSERIAL and BIGSERIAL?
        //TODO: maybe check given type to be of integer?
        serial = true;
    } else {
        sqlType = column.sqlType();
    }
    // serials are already NOT NULL
    if (!serial && !column.isNullable()) {
        sqlColumnOptions << "NOT NULL";
    }
    if (column.isUnique()) {
        sqlColumnOptions << "UNIQUE";
    }
    if (column.hasDefaultValue()) {
        sqlColumnOptions << QString("DEFAULT (%1)").arg(column.defaultValue());
    }

    return QString("%1 %2 %3").arg(column.name(), sqlType, sqlColumnOptions.join(" "));
}

} // namespace Helper
