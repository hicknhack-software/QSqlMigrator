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
#include "MysqlMigrator/Helper/MysqlStructureService.h"

#include "MigrationTracker/MigrationTrackerService.h"

#include "CommandExecution/CommandExecutionContext.h"

#include "Structure/Table.h"
#include "Structure/Index.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

using namespace Structure;

namespace Helper {

MysqlStructureService::MysqlStructureService()
{
}

Table MysqlStructureService::getTableDefinition(const QString &tableName
                                          , QSqlDatabase database) const
{
    ColumnList columns;
    QString queryString = QString("DESCRIBE %1").arg(tableName);
    QSqlQuery query = database.exec(queryString);
    QSqlError error = query.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
        while (query.next()) {
            QString name = query.value(0).toString();
            QString type = query.value(1).toString();
            bool null = query.value(2).toBool();
            QString key = query.value(3).toString();
            QString defaultValue = query.value(4).toString();
            QString extra = query.value(5).toString();

            Column::Attributes attr = Column::None;
            if (!null) {
                attr |= Column::NotNullable;
            }
            if (key == "PRI") {
                attr |= Column::Primary;
            } else if (key == "UNI") {
                attr |= Column::Unique;
            }
            if (extra == "auto_increment") {
                attr |= Column::AutoIncrement;
            }

            //TODO: primary keys get default value set as 0?
            if (key == "PRI" && defaultValue == "0") {
                defaultValue = "";
            }
            columns << Column(name, type, defaultValue, attr);
        }
    }
    return Table(tableName, columns);
}

Index MysqlStructureService::getIndexDefinition(const QString &indexName, const QString &tableName, QSqlDatabase database) const
{
    Structure::Index::ColumnList columns;
    QString queryText = QString("SHOW INDEXES FROM %1 WHERE Key_name = \"%2\"").arg(tableName, indexName);
    ::qDebug() << "query looks like: " << queryText;
    QSqlQuery query = database.exec(queryText);
    QSqlError error = query.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
        while (query.next()) {
            columns << query.value(4).toString();
        }
    }
    return Structure::Index(indexName, tableName, columns);
}

} // namespace Helper
