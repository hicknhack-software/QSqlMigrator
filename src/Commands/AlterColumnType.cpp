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
#include "AlterColumnType.h"

#include <QDebug>

namespace Commands {

AlterColumnType::AlterColumnType(const QString &columnName, const QString &tableName
                                 , const Structure::Type &newType, const Structure::Type &oldType)
    : BaseCommand(AlterColumnType::typeName())
    , m_tableName(tableName)
    , m_columnName(columnName)
    , m_newType(newType)
    , m_oldType(oldType)
{
    if (columnName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command with empty columnName!";
    }
    if (tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command" << columnName << "with empty tableName!";
    }
    if (!m_newType.isValid()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command" << columnName << "with invalid new type!" << tableName;
    }
    if (!m_oldType.isValid()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command" << columnName << "with invalid old type!" << tableName;
    }
}

AlterColumnType::AlterColumnType(const QString &columnName, const QString &tableName, const Structure::Type &newType)
    : BaseCommand(AlterColumnType::typeName())
    , m_tableName(tableName)
    , m_columnName(columnName)
    , m_newType(newType)
    , m_oldType(Structure::Type::invalid())
{
    if(columnName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command with empty columnName!" << tableName;
    }
    if(tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command" << columnName << "with empty tableName!";
    }
    if (!m_newType.isValid()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command" << columnName << "with invalid new type!" << tableName;
    }
}

const QString &AlterColumnType::typeName()
{
    static QString typeName("AlterColumnType");
    return typeName;
}

CommandPtr AlterColumnType::reverse() const
{
    if (!m_oldType.isValid())
        return CommandPtr();
    return CommandPtr(new AlterColumnType(columnName(), tableName(), oldType(), newType()));
}

} // namespace Commands
