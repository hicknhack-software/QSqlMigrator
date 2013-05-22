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
#include "Database.h"

#include <QDebug>

namespace Scheme {

Database &Database::add(const DbTable &newTable)
{
    foreach(const DbTable &table, m_tables) {
        if(table.name() == newTable.name()) {
            ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "Table" << newTable.name() << "already contained in database!";
            return (*this);
        }
    }

    m_tables << newTable;
    return (*this);
}

Database &Database::removeTable(const QString &tableName)
{
    QMutableListIterator<DbTable> tableIterator(m_tables);
    while(tableIterator.hasNext()) {
        tableIterator.next();
        const DbTable &table = tableIterator.value();
        if(table.name() == tableName) {
            tableIterator.remove();
            break;
        }
    }

    return (*this);
}

}
