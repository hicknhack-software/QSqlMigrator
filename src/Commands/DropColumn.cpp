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
#include "DropColumn.h"

#include "Commands/AddColumn.h"

#include <QDebug>

namespace Commands {

DropColumn::DropColumn(const QString &columnName, const QString &tableName)
    : BaseCommand(DropColumn::typeName())
    , m_column(Structure::Column(columnName, Structure::Type::invalid()))
    , m_tableName(tableName)
{
    if(columnName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << DropColumn::typeName() << "command with empty columnName!";
    }
    if(tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << DropColumn::typeName() << "command" << columnName << "with empty tableName!";
    }
}

DropColumn::DropColumn(const Structure::Column &column, const QString &tableName)
    : BaseCommand(DropColumn::typeName())
    , m_column(column)
    , m_tableName(tableName)
{
    if(! column.isValid()) {
        ::qWarning() << LOG_PREFIX << DropColumn::typeName() << "command with invalid column!";
    }
    if(tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << DropColumn::typeName() << "command" << column.name() << "with empty tableName!";
    }
}

const QString &DropColumn::typeName()
{
    static QString typeName("DropColumn");
    return typeName;
}

CommandPtr DropColumn::reverse() const
{
    if (!m_column.isValid())
        return CommandPtr();
    return CommandPtr(new AddColumn(column(), tableName()));
}

} // namespace Commands
