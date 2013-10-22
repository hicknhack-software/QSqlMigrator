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
#include "BaseSqlMigrator/Helper/BaseSqlColumnService.h"

#include <QStringList>

using namespace Structure;

namespace Helper {

BaseSqlColumnService::BaseSqlColumnService()
{
}

QString BaseSqlColumnService::generateColumnsDefinitionSql(const QList<Column> &columnList) const
{
    QStringList sqlColumnDefinitions;
    foreach (Column column, columnList) {
        sqlColumnDefinitions << generateColumnDefinitionSql(column);
    }
    return sqlColumnDefinitions.join(", ");
}

QString BaseSqlColumnService::generateIndexColumnDefinitionSql(const QString &columnName
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

QString BaseSqlColumnService::generateIndexColumnDefinitionSql(const Index::IndexHash &columns) const
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
