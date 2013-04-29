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

#include <QStringList>

using namespace Structure;

namespace Helper {

SqliteColumnService::SqliteColumnService()
{
}

const QString SqliteColumnService::generateColumnDefinitionSql(const Column &column) const
{
    QStringList sqlColumnOptions;
    if (!column.isNullable()) {
        sqlColumnOptions << "NOT NULL";
    }
    if (column.isPrimary()) {
        sqlColumnOptions << "PRIMARY KEY";
        //primary key could be set to autoincrement
        if (column.attributes().testFlag(Column::AutoIncrement)) {
            sqlColumnOptions << "AUTOINCREMENT";
        }
    }
    if (column.isUnique()) {
        sqlColumnOptions << "UNIQUE";
    }
    if (column.hasDefaultValue()) {
        sqlColumnOptions << QString("DEFAULT (%1)").arg(column.defaultValue());
    }

    return QString("%1 %2 %3").arg(column.name(), column.sqlType(), sqlColumnOptions.join(" "));
}

const QString SqliteColumnService::generateColumnDefinitionSql(const QList<Column> &columnList) const
{
    QStringList sqlColumnDefinitions;
    foreach (Column column, columnList) {
        sqlColumnDefinitions << this->generateColumnDefinitionSql(column);
    }
    return sqlColumnDefinitions.join(", ");
}

const QString SqliteColumnService::generateIndexColumnDefinitionSql(const QString &columnName
                                                                      , const Index::SortOrder &sortOrder) const
{
    QString sqlSortOrder;
    switch (sortOrder) {
    case Index::Ascending:
        sqlSortOrder = "ASC";
        break;
    case Index::Descending:
        sqlSortOrder = "DESC";
        break;
    default:
        sqlSortOrder = "";
        break;
    }

    return QString("%1 %2").arg(columnName, sqlSortOrder);
}

const QString SqliteColumnService::generateIndexColumnDefinitionSql(const Index::IndexHash &columns) const
{
    QStringList sqlIndexColumnDefinitions;
    QHashIterator<QString, Index::SortOrder> i(columns);
    while (i.hasNext()) {
        i.next();
        sqlIndexColumnDefinitions << this->generateIndexColumnDefinitionSql(i.key(), i.value());
    }
    return sqlIndexColumnDefinitions.join(", ");
}

} // namespace Helper
