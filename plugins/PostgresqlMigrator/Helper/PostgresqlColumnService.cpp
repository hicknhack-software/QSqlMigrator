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

#include "PostgresqlMigrator/Helper/PostgresqlTypeMapperService.h"

#include "MigrationTracker/MigrationTrackerService.h"

#include <QStringList>

#include <QDebug>

using namespace Structure;

namespace Helper {

PostgresqlColumnService::PostgresqlColumnService(const PostgresqlTypeMapperService &postgresqlTypeMapperService)
    : BaseSqlColumnService(postgresqlTypeMapperService)
{
}

QString PostgresqlColumnService::buildColumnTypeSql(const Column &column) const
{
    if (column.type().isString()) {
        static const QStringList serialTypes = QStringList() << "SMALLSERIAL" << "SERIAL" << "BIGSERIAL";
        if (column.isAutoIncremented() && serialTypes.contains( column.type().string(), Qt::CaseInsensitive ) ) {
            qWarning() << "column" << column.name() << "has auto increment specified but is not a serial type";
        }
    }
    else if (column.isAutoIncremented()) {
        // PostgreSQL has no auto increment, instead there are serials (as types)
        // QVariant has no Short
        switch(column.type().base())
        {
        case Type::SmallInt:
            return "SMALLSERIAL";

        case Type::Integer:
            return "SERIAL";

        case Type::BigInt:
            return "BIGSERIAL";

        default:
            qWarning() << "column" << column.name() << "has auto increment specified but is not of integer type";
            return "SERIAL";
        }
    }
    return BaseSqlColumnService::buildColumnTypeSql(column);
}

void PostgresqlColumnService::buildColumnOptionsSql(const Column &column,
                                                    const BaseSqlColumnService::StringOutputFunction &addOption) const
{
    if (column.isPrimary()) {
        addOption("PRIMARY KEY");
    }
    else if (!column.isAutoIncremented() && !column.isNullable()) {
        addOption("NOT NULL");
    }
    if (column.isUnique()) {
        addOption("UNIQUE");
    }
    if (column.hasDefaultValue()) {
        addOption(QString("DEFAULT (%1)").arg(column.defaultValue()));
    }
}

} // namespace Helper
