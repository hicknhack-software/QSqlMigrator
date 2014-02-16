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
#include "PostgresqlMigrator/Helper/PostgresqlStructureService.h"

#include "Structure/Table.h"
#include "Structure/Index.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>

using namespace Structure;

namespace Helper {

PostgresqlStructureService::PostgresqlStructureService()
{}

Table PostgresqlStructureService::getTableDefinition(const QString &tableName,
                                                     QSqlDatabase database) const
{
    ColumnList columns;

    enum { NAME, TYPE, DEFAULT, NOT_NULL, CONSTRAINT };
    const QString columnQueryString =
            QString("SELECT attname, pg_catalog.format_type(atttypid, atttypmod),"
                    " pg_catalog.pg_get_expr(adbin, adrelid), attnotnull, conname"
                    " FROM pg_catalog.pg_class INNER JOIN pg_catalog.pg_attribute ON attrelid = pg_class.oid"
                    " LEFT JOIN pg_catalog.pg_attrdef ON adrelid = attrelid AND adnum = attnum AND atthasdef"
                    " LEFT JOIN pg_catalog.pg_constraint ON conrelid = pg_class.oid AND attnum = ANY(conkey)"
                    " WHERE pg_class.relname = '%1' AND pg_catalog.pg_table_is_visible(pg_class.oid)"
                    " AND attnum > 0 AND atttypid <> 0::oid AND NOT attisdropped ORDER BY attnum")
            .arg(tableName);
    //    ::qDebug() << "query looks like: " << columnQueryString;
    QSqlQuery queryColumns = database.exec(columnQueryString);
    QSqlError sqlError = queryColumns.lastError();
    if (sqlError.isValid()) {
        ::qDebug() << Q_FUNC_INFO << sqlError.text();
        return Table(tableName, columns);
    }

    while (queryColumns.next()) {
        const QString name = queryColumns.value(NAME).toString();
        const QString type = queryColumns.value(TYPE).toString();
        QString defaultValue = queryColumns.value(DEFAULT).toString();
        const bool notNull = queryColumns.value(NOT_NULL).toBool();
        const QString constraint = queryColumns.value(CONSTRAINT).toString();

        Column::Attributes attr = Column::None;
        if (notNull) {
            attr |= Column::NotNullable;
        }
        if (constraint == "prim" || constraint.contains("pkey")) {
            attr |= Column::Primary;
        } else if (constraint == "uniq" || constraint.contains("key")) {
            attr |= Column::Unique;
        }
        if (defaultValue == QString("nextval('%1_%2_seq'::regclass)").arg(tableName, name)) {
            attr |= Column::AutoIncrement;
            defaultValue.clear();
        }

        columns << Column(name, type, defaultValue, attr);
    }
    return Table(tableName, columns);
}

Index PostgresqlStructureService::getIndexDefinition(const QString &indexName, const QString &tableName, QSqlDatabase database) const
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
