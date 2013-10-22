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
#include "SqliteColumnService.h"

#include "BaseSqlMigrator/Helper/BaseSqlTypeMapperService.h"

#include <QStringList>

#include <QDebug>

using namespace Structure;

namespace Helper {

SqliteColumnService::SqliteColumnService()
{
}

QString SqliteColumnService::generateColumnDefinitionSql(const Column &column) const
{
    QStringList sqlColumnOptions;
    // PRIMARY KEY implies NOT NULL
    if (!column.isNullable() && !column.isPrimary()) {
        sqlColumnOptions << "NOT NULL";
    }
    if (column.isPrimary()) {
        // primary key may be automatically incremented
        if (column.isAutoIncremented()) {
            // in SQLite, the column of type INTEGER which is a PRIMARY KEY is an alias to ROWID,
            // which is a special column with AUTOINCREMENT
            sqlColumnOptions << "PRIMARY KEY";
            //sqlColumnOptions << "AUTOINCREMENT"; //TODO: is this needed?
        }
    }
    if (column.isUnique()) {
        sqlColumnOptions << "UNIQUE";
    }
    if (column.hasDefaultValue()) {
        sqlColumnOptions << QString("DEFAULT (%1)").arg(column.defaultValue());
    }

    QString sqlTypeString;
    if (column.hasSqlTypeString())
        sqlTypeString = column.sqlTypeString();
    else {
        BaseSqlTypeMapperService typeMapperService;
        sqlTypeString = typeMapperService.map(column.sqlType());
    }
    return QString("%1 %2 %3").arg(column.name(), sqlTypeString, sqlColumnOptions.join(" "));
}

} // namespace Helper
