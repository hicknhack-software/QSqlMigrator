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
#include "SqliteMigrator/Helper/SqliteSqlStructureService.h"

#include "Structure/Table.h"
#include "Structure/Index.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>

using namespace Structure;

namespace Helper {

SqliteSqlStructureService::SqliteSqlStructureService()
{}

Table SqliteSqlStructureService::getTableDefinition(const QString &tableName,
                                                    QSqlDatabase database) const
{
    ColumnList columns;
    QString queryString = QString("PRAGMA table_info(%1)").arg(tableName);
    QSqlQuery columnsQuery = database.exec(queryString);
    QSqlError error = columnsQuery.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
        return Table(tableName, columns);
    }

    queryString = QString("PRAGMA index_list(%1)").arg(tableName);
    QSqlQuery queryIndexList = database.exec(queryString);
    error = queryIndexList.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
        return Table(tableName, columns);
    }
    QSet<QString> uniqueColumns;
    while (queryIndexList.next()) {
        bool unique = queryIndexList.value(2).toBool();
        if (!unique)
            continue;

        QString indexName = queryIndexList.value(1).toString();

        queryString = QString("PRAGMA index_info(%1)").arg(indexName);
        QSqlQuery queryIndexInfo = database.exec(queryString);
        error = queryIndexInfo.lastError();
        if (error.isValid()) {
            ::qDebug() << Q_FUNC_INFO << error.text();
            continue;
        }
        while (queryIndexInfo.next()) {
            QString column(queryIndexInfo.value(2).toString());
            uniqueColumns.insert( column );
        }
    }

    while (columnsQuery.next()) {
        QString name = columnsQuery.value(1).toString();
        QString type = columnsQuery.value(2).toString();
        bool notNull = columnsQuery.value(3).toBool();
        QString defaultValue = columnsQuery.value(4).toString();
        bool primaryKey = columnsQuery.value(5).toBool();

        Column::Attributes attr = Column::None;
        if (notNull) {
            attr |= Column::NotNullable;
        }
        if (primaryKey) {
            attr |= Column::Primary;
            if (type.startsWith("INTEGER", Qt::CaseInsensitive))
                attr |= Column::AutoIncrement;
        }
        if (uniqueColumns.contains(name))
            attr |= Column::Unique;

        columns << Column(name, type, defaultValue, attr);
    }
    return Table(tableName, columns);
}

Structure::Index SqliteSqlStructureService::getIndexDefinition(const QString &indexName,
                                                               const QString &tableName,
                                                               QSqlDatabase database) const
{
    Structure::Index::ColumnList columns;
    QString sqlText;
    QString queryString = QString("SELECT sql FROM sqlite_master WHERE name = '%1'").arg(indexName);
    qDebug() << "complete query looks like: " << queryString;
    QSqlQuery query = database.exec(queryString);
    QSqlError error = query.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
        // should return one row with one column
        query.next();
        sqlText = query.value(0).toString();
    }

    // hacky - parse the SQL statement for creating the index, look for ASC or DESC keyword
    int openParen = sqlText.indexOf("(");
    int closeParen = sqlText.lastIndexOf(")");
    QStringList columnTextList = sqlText.mid(openParen+1, closeParen-openParen-1).split(",");
    foreach (const QString &columnText, columnTextList) {
        if (columnText.contains("ASC", Qt::CaseInsensitive))
            columns << Structure::Index::Column(columnText.simplified().split(" ")[0], Index::Ascending);
        else if (columnText.contains("DESC", Qt::CaseInsensitive))
            columns << Structure::Index::Column(columnText.simplified().split(" ")[0], Index::Descending);
        else
            columns << columnText.split(" ")[0];
    }

    //    QString queryString = QString("PRAGMA index_info(%1)").arg(indexName);
    //    QSqlQuery query = database.exec(queryString);
    //    QSqlError error = query.lastError();
    //    if (error.isValid()) {
    //        ::qDebug() << Q_FUNC_INFO << error.text();
    //    } else {
    //        while (query.next()) {
    //            columns << query.value(2).toString();
    //        }
    //    }
    return Structure::Index(indexName, tableName, columns);
}

} // namespace Helper
