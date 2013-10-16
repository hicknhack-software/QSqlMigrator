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

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

using namespace Structure;

namespace Helper {

PostgresqlDbReaderService::PostgresqlDbReaderService()
{}

PostgresqlDbReaderService::~PostgresqlDbReaderService()
{}

Table PostgresqlDbReaderService::getTableDefinition(const QString &tableName
                                        , QSqlDatabase database) const
{
    Table table = Table(tableName);
    QString queryString = QString(
                "SELECT "
                "    columns.column_name, "
                "    udt_name, "
                "    character_maximum_length, "
                "    is_nullable, "
                "    column_default, "
                "    constraints.constraint_type "
                "FROM "
                "    information_schema.columns LEFT JOIN "
                "    ( "
                "        SELECT "
                "            b.column_name, "
                "            a.constraint_type "
                "        FROM "
                "            information_schema.table_constraints A, "
                "            information_schema.constraint_column_usage B "
                "        WHERE "
                "            a.constraint_name = b.constraint_name AND "
                "            a.table_name = '%1' "
                "    ) AS constraints "
                "    ON constraints.column_name = columns.column_name "
                "WHERE "
                "    table_name ='%1' "
                ";").arg(tableName);
    QSqlQuery query = database.exec(queryString);
    QSqlError error = query.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
        while (query.next()) {
            QString name = query.value(0).toString();
            QString type = query.value(1).toString();
            QString typeLength = query.value(2).toString();
            if (typeLength != "0")
                type = QString("%1(%2)").arg(type, typeLength);
            bool null = query.value(3).toBool();
            QString defaultValue = query.value(4).toString();
            QString key = query.value(5).toString();

            Column::Attributes attr = Column::None;
            if (!null) {
                attr |= Column::NotNullable;
            }
            if (key == "PRIMARY KEY") {
                attr |= Column::Primary;
            } else if (key == "UNIQUE") {
                attr |= Column::Unique;
            }
            if (defaultValue == QString("nextval('%1_%2_seq'::regclass)")) {
                defaultValue = "";
                attr |= Column::AutoIncrement;
            }

            // on PostgreSQL, default value field then contains something like: nextval('tablename_identifier_seq'::regclass)

            Column col = Column(name, type, attr);
            if (!defaultValue.isEmpty()) {
                col.setDefault(defaultValue);
            }
            table.add(col);
        }
    }
    return table;
}

} // namespace Helper
