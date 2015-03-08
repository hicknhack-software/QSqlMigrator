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
#include "SqliteSchemaReflection.h"

#include "SqlSchema/Table.h"
#include "SqlSchema/Index.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>

namespace SqliteMigrator {

namespace {
QSet<QString> extractUniqueColumns(QSqlQuery &indexListQuery, QSqlDatabase database)
{
    QSet<QString> uniqueColumns;
    while (indexListQuery.next()) {
        bool unique = indexListQuery.value(2).toBool();
        if (!unique)
            continue;

        QString indexName = indexListQuery.value(1).toString();

        QString queryString = QString("PRAGMA index_info(%1)").arg(indexName);
        QSqlQuery indexInfoQuery = database.exec(queryString);
        QSqlError error = indexInfoQuery.lastError();
        if (error.isValid()) {
            ::qDebug() << Q_FUNC_INFO << error.text();
            continue;
        }
        while (indexInfoQuery.next()) {
            QString column(indexInfoQuery.value(2).toString());
            uniqueColumns.insert( column );
        }
    }
    return uniqueColumns;
}

QSqlMigrator::SqlSchema::Table::Columns columnsFromTableInfo(QSqlQuery &tableInfoQuery, const QSet<QString> &uniqueColumns)
{
    using namespace QSqlMigrator::SqlSchema;

    Table::Columns tableColumns;
    tableColumns.reserve(tableInfoQuery.size());
    while (tableInfoQuery.next()) {
        QString name = tableInfoQuery.value(1).toString();
        QString type = tableInfoQuery.value(2).toString();
        bool notNull = tableInfoQuery.value(3).toBool();
        QString defaultValue = tableInfoQuery.value(4).toString();
        bool primaryKey = tableInfoQuery.value(5).toBool();

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

        tableColumns << Column(name, ValueType(type), defaultValue, attr);
    }
    return tableColumns;
}

QSqlMigrator::SqlSchema::Index::Columns indexColumnsFromCreateIndexSql(const QString &sqlText)
{
    using namespace QSqlMigrator::SqlSchema;

    // hacky - parse the SQL statement for creating the index, look for ASC or DESC keyword
    int openParen = sqlText.indexOf("(");
    int closeParen = sqlText.lastIndexOf(")");
    QStringList columnTextList = sqlText.mid(openParen+1, closeParen-openParen-1).split(",");
    Index::Columns indexColumns;
    foreach (const QString &columnText, columnTextList) {
        if (columnText.contains("ASC", Qt::CaseInsensitive))
            indexColumns << IndexColumn(columnText.simplified().split(" ")[0], SortOrder::Ascending);
        else if (columnText.contains("DESC", Qt::CaseInsensitive))
            indexColumns << IndexColumn(columnText.simplified().split(" ")[0], SortOrder::Descending);
        else
            indexColumns << columnText.split(" ")[0];
    }
    return indexColumns;
}

} // local namespace

SqliteSchemaReflection::Table
SqliteSchemaReflection::getTableDefinition(const TableName &tableName,
                                           SqlDatabase database) const
{
    Table::Columns tableColumns;
    do {
        QString queryString = QString("PRAGMA table_info(%1)").arg(tableName);
        QSqlQuery tableInfoQuery = database.exec(queryString);
        QSqlError error = tableInfoQuery.lastError();
        if (error.isValid()) {
            ::qDebug() << Q_FUNC_INFO << error.text();
            break;
        }

        queryString = QString("PRAGMA index_list(%1)").arg(tableName);
        QSqlQuery indexListQuery = database.exec(queryString);
        error = indexListQuery.lastError();
        if (error.isValid()) {
            ::qDebug() << Q_FUNC_INFO << error.text();
            break;
        }

        QSet<QString> uniqueColumns = extractUniqueColumns(indexListQuery, database);
        tableColumns = columnsFromTableInfo(tableInfoQuery, uniqueColumns);
    }
    while(false);
    return Table(tableName, tableColumns);
}

SqliteSchemaReflection::Index
SqliteSchemaReflection::getIndexDefinition(const IndexName &indexName,
                                           const TableName &tableName,
                                           SqlDatabase database) const
{
    Index::Columns indexColumns;
    do {
        QString queryString = QString("SELECT sql FROM sqlite_master WHERE name = '%1'").arg(indexName);
        qDebug() << "complete query looks like: " << queryString;
        QSqlQuery query = database.exec(queryString);
        QSqlError error = query.lastError();
        if (error.isValid()) {
            ::qDebug() << Q_FUNC_INFO << error.text();
            break;
        }
        bool success = query.next();
        if (!success) {
            ::qDebug() << Q_FUNC_INFO << "no results";
            break;
        }
        QString sqlText = query.value(0).toString();

        indexColumns = indexColumnsFromCreateIndexSql(sqlText);
    } while(false);

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
    return Index(indexName, tableName, indexColumns);
}

} // namespace SqliteMigrator
