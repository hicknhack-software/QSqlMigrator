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
#include "SqliteMigrator/Helper/SqliteDbReader.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

using namespace Structure;

namespace Helper {

SqliteDbReader::SqliteDbReader()
{
}

Table SqliteDbReader::getTableDefinition(const QString &tableName
                                         ,const CommandExecution::CommandExecutionContext &context) const
{
    Table table = Table(tableName);
    QString sQueryString = QString("PRAGMA table_info(%1)").arg(tableName);
    QSqlQuery query = context.database().exec(sQueryString);
    QSqlError error = query.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
        while (query.next()) {
            QString sName = query.value(1).toString();
            QString sType = query.value(2).toString();
            bool bNotNull = query.value(3).toBool();
            QString sDefaultValue = query.value(4).toString();
            bool bPrimaryKey = query.value(5).toBool();

            Column::Attributes attr = Column::None;
            if (bNotNull) {
                attr |= Column::NotNullable;
            }
            if (bPrimaryKey) {
                attr |= Column::Primary;
            }

            Column col = Column(sName, sType, attr);
            if (!sDefaultValue.isEmpty()) {
                col.setDefault(sDefaultValue);
            }
            table.add(col);
        }
    }
    return table;
}

} // namespace Helper
