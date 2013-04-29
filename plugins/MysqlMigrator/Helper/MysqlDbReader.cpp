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
#include "MysqlDbReader.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

using namespace Structure;

namespace Helper {

MysqlDbReader::MysqlDbReader()
{
}

const Structure::Table MysqlDbReader::getTableDefinition(const QString &tableName
                                                         , CommandExecution::CommandExecutionContext &context) const
{
    Table table = Table(tableName);
    QString sQueryString = QString("DESCRIBE %1").arg(tableName);
    QSqlQuery query = context.database().exec(sQueryString);
    QSqlError error = query.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
        while (query.next()) {
            QString sName = query.value(0).toString();
            QString sType = query.value(1).toString();
            bool bNull = query.value(2).toBool();
            QString sKey = query.value(3).toString();
            QString sDefaultValue = query.value(4).toString();
            QString sExtra = query.value(5).toString();

            Column::Attributes attr = Column::None;
            if (!bNull) {
                attr |= Column::NotNullable;
            }
            if (sKey == "PRI") {
                attr |= Column::Primary;
            } else if (sKey == "UNI") {
                attr |= Column::Unique;
            }
            if (sExtra == "auto_increment") {
                attr |= Column::AutoIncrement;
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
