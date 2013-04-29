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
#include "AddColumn.h"

#include <QDebug>

namespace Commands {

AddColumn::AddColumn(const Structure::Column &column, const QString &tableName)
    : BaseCommand(AddColumn::typeName())
    , m_column(column)
    , m_tableName(tableName)
{
    if(tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << AddColumn::typeName() << "command" << column.name() << "with empty tableName!";
    }
}

const QString &AddColumn::typeName()
{
    static QString sTypename("AddColumn");
    return sTypename;
}

const QString &AddColumn::tableName() const
{
    return m_tableName;
}

const Structure::Column &AddColumn::column() const
{
    return m_column;
}

} // namespace Commands
