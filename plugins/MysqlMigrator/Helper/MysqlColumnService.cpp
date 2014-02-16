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

#include "MysqlColumnService.h"

#include "MigrationTracker/MigrationTrackerService.h"

#include "MysqlMigrator/Helper/MysqlTypeMapperService.h"

#include <QStringList>

#include <QDebug>

using namespace Structure;

namespace Helper {

MysqlColumnService::MysqlColumnService(const MysqlTypeMapperService &mysqlTypeMapperService)
    : BaseSqlColumnService(mysqlTypeMapperService)
{
}

QStringList MysqlColumnService::buildColumnOptionsSql(const Column &column) const
{
    QStringList sqlColumnOptions;
    // PRIMARY KEY implies NOT NULL
    if (!column.isPrimary() && !column.isNullable()) {
        sqlColumnOptions << "NOT NULL";
    } // default is NULL for columns
    if (column.isPrimary()) {
        // TODO: handle PRIMARY KEY in function which call this one, so compound keys are handled easily
        sqlColumnOptions << "PRIMARY KEY";
        if (column.isAutoIncremented()) { // primary key may be automatically incremented
            sqlColumnOptions << "AUTO_INCREMENT";
        }
    } else if (column.isAutoIncremented()) {
        // in MySQL there can be only one auto column and it must be defined as a key
        ::qWarning() << "warning: column" << column.name()
                 << ": in MySQL there can be only one auto column and it must be defined as a key";
    }
    if (column.isUnique()) {
        sqlColumnOptions << "UNIQUE";
    }
    if (column.hasDefaultValue()) {
        sqlColumnOptions << QString("DEFAULT (%1)").arg(column.defaultValue());
    }
    return sqlColumnOptions;
}

} // namespace Helper
