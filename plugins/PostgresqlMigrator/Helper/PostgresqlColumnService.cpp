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

QString PostgresqlColumnService::generateColumnDefinitionSql(const Column &column) const
{
    QString sqlTypeString;
    if (column.type().isString()) {
        sqlTypeString = column.type().string();

        static const QStringList serialTypes = QStringList() << "SMALLSERIAL" << "SERIAL" << "BIGSERIAL";
        if (column.isAutoIncremented() && serialTypes.contains( sqlTypeString, Qt::CaseInsensitive ) ) {
            qWarning() << "column" << column.name() << "has auto increment specified but is not a serial type";
        }
    }
    else {
        if (column.isAutoIncremented()) {
            // PostgreSQL has no auto increment, instead there are serials (as types)
            // QVariant has no Short
            if (column.type().base() == Type::Integer)
                sqlTypeString = "SERIAL";
            else if (column.type().base() == Type::BigInt)
                sqlTypeString = "BIGSERIAL";
            else {
                qWarning() << "column" << column.name() << "has auto increment specified but is not of integer type";
                sqlTypeString = "SERIAL";
            }
        } else {
            sqlTypeString = m_typeMapperService.map(column.type());
        }
    }

    QStringList sqlColumnOptions;
    if (column.isPrimary()) {
        sqlColumnOptions << "PRIMARY KEY";
    }
    else if (!column.isAutoIncremented() && !column.isNullable()) {
        sqlColumnOptions << "NOT NULL";
    }
    if (column.isUnique()) {
        sqlColumnOptions << "UNIQUE";
    }
    if (column.hasDefaultValue()) {
        sqlColumnOptions << QString("DEFAULT (%1)").arg(column.defaultValue());
    }

    return QString("%1 %2 %3").arg(column.name(), sqlTypeString, sqlColumnOptions.join(" "));
}

} // namespace Helper
