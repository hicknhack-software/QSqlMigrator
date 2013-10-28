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
#include "PostgresqlMigrator/Helper/PostgresqlDbReaderService.h"

#include "Structure/Table.h"
#include "Structure/Index.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>

using namespace Structure;

namespace Helper {

PostgresqlDbReaderService::PostgresqlDbReaderService()
{}

PostgresqlDbReaderService::~PostgresqlDbReaderService()
{}

Table PostgresqlDbReaderService::getTableDefinition(const QString &tableName
                                        , QSqlDatabase database) const
{
    ColumnList columns;
    QString oid;
    // select oid
    QString queryString = QString(
                "SELECT c.oid "
                "FROM pg_catalog.pg_class c "
                "WHERE c.relname ~ '^(%1)$' "
                /**/"AND pg_catalog.pg_table_is_visible(c.oid) "
                ";"
                ).arg(tableName);
    ::qDebug() << "query looks like: " << queryString;
    QSqlQuery queryOid = database.exec(queryString);
    QSqlError error = queryOid.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
        // returns only one row with one column
        queryOid.next();
        oid = queryOid.value(0).toString();

        // select information about columns (without type!)
        queryString = QString(
                    "SELECT "
                    /**/"columns.column_name, "
                    //"    udt_name, "
                    //"    character_maximum_length, "
                    /**/"is_nullable, "
                    /**/"column_default, "
                    /**/"constraints.constraint_type "
                    "FROM "
                    /**/"information_schema.columns LEFT JOIN "
                    /**/"( "
                    /* * */ "SELECT "
                    /* * * * */ "b.column_name, "
                    /* * * * */ "a.constraint_type "
                    /* * */ "FROM "
                    /* * * * */ "information_schema.table_constraints A, "
                    /* * * * */ "information_schema.constraint_column_usage B "
                    /* * */ "WHERE "
                    /* * * * */ "a.constraint_name = b.constraint_name AND "
                    /* * * * */ "a.table_name = '%1' "
                    /**/") AS constraints "
                    /**/"ON constraints.column_name = columns.column_name "
                    "WHERE "
                    /**/"table_name ='%1' "
                    ";").arg(tableName);
        ::qDebug() << "query looks like: " << queryString;
        QSqlQuery queryColumns = database.exec(queryString);
        error = queryColumns.lastError();
        if (error.isValid()) {
            ::qDebug() << Q_FUNC_INFO << error.text();
        } else {
            while (queryColumns.next()) {
                QString name = queryColumns.value(0).toString();

                //TODO: select all types at once?
                // select type of column of table given by oid
                queryString = QString(
                            "SELECT "
                            /**/"pg_catalog.format_type(a.atttypid, a.atttypmod) "
                            "FROM pg_catalog.pg_attribute a "
                            "WHERE a.attrelid = '%1' AND a.attnum > 0 AND NOT a.attisdropped AND a.attname = '%2' "
                            ";"
                            ).arg(oid, name);
                ::qDebug() << "query looks like: " << queryString;
                QSqlQuery queryColumnType = database.exec(queryString);
                error = queryColumnType.lastError();
                if (error.isValid()) {
                    ::qDebug() << Q_FUNC_INFO << error.text();
                }
                // return one row with one column
                queryColumnType.next();
                QString type = queryColumnType.value(0).toString();

                bool null = queryColumns.value(1).toBool();
                QString defaultValue = queryColumns.value(2).toString();
                QString key = queryColumns.value(3).toString();

                Column::Attributes attr = Column::None;
                if (!null) {
                    attr |= Column::NotNullable;
                }
                if (key == "PRIMARY KEY") {
                    attr |= Column::Primary;
                } else if (key == "UNIQUE") {
                    attr |= Column::Unique;
                }
                if (defaultValue == QString("nextval('%1_%2_seq'::regclass)").arg(tableName, name)) {
                    defaultValue = "";
                    attr |= Column::AutoIncrement;
                }  // on PostgreSQL, default value field then contains something like: nextval('tablename_identifier_seq'::regclass)

                columns << Column(name, type, defaultValue, attr);
            }
        }
    }
    return Table(tableName, columns);

}

Index PostgresqlDbReaderService::getIndexDefinition(const QString &indexName, const QString &tableName, QSqlDatabase database) const
{
    // select index keys order and options (both vectors)

    Structure::Index::ColumnList columns;
    QStringList indkey;
    QStringList indoption;
    QString queryIndexText = QString(
                "select "
                /**/"ix.indkey, ix.indoption "
                "from "
                /**/"pg_class t, "
                /**/"pg_class i, "
                /**/"pg_index ix "
                "where "
                /**/"t.oid = ix.indrelid "
                /**/"and i.oid = ix.indexrelid "
                /**/"and t.relkind = 'r' "
                /**/"and t.relname = '%1' "
                /**/"and i.relname = '%2' "
                /**/";").arg(tableName, indexName);
    qDebug() << "complete query looks like: " << queryIndexText;
    QSqlQuery queryIndex = database.exec(queryIndexText);
    QSqlError error = queryIndex.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
        queryIndex.next();
        indkey = queryIndex.value(0).toString().split(" ");
        indoption = queryIndex.value(1).toString().split(" ");

        QString queryColumnsText = QString(
                    "select "
                    /**/"a.attname, "
                    /**/"a.attnum "
                    "from "
                    /**/"pg_class t, "
                    /**/"pg_class i, "
                    /**/"pg_index ix, "
                    /**/"pg_attribute a "
                    "where "
                    /**/"t.oid = ix.indrelid "
                    /**/"and i.oid = ix.indexrelid "
                    /**/"and a.attrelid = t.oid "
                    /**/"and a.attnum = ANY(ix.indkey) "
                    /**/"and t.relkind = 'r' "
                    /**/"and t.relname = '%1' "
                    /**/"and i.relname = '%2' "
                    "order by "
                    /**/"a.attnum "
                    ";").arg(tableName, indexName);
        qDebug() << "complete query looks like: " << queryColumnsText;
        QSqlQuery queryColumns = database.exec(queryColumnsText);
        error = queryColumns.lastError();
        if (error.isValid()) {
            ::qDebug() << Q_FUNC_INFO << error.text();
        } else {
            QMap<QString, QString> columnsNamesMap;
            while (queryColumns.next()) {
                QString name = queryColumns.value(0).toString();
                QString attnum = queryColumns.value(1).toString();
                columnsNamesMap[attnum] = name;
            }
            int i=0;
            foreach (const QString &attnum, indkey) {
                Structure::Index::SortOrder sortOrder = Structure::Index::Default;
                if (indoption.at(i).toInt() & 1)
                    sortOrder = Structure::Index::Descending;
                columns << Structure::Index::Column(columnsNamesMap[attnum], sortOrder);
                ++i;
            }
        }
    }
    return Structure::Index(indexName, tableName, columns);
}

} // namespace Helper
