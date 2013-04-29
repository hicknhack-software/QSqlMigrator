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
                                 , const QString &newType, const QString &oldType)
    : BaseCommand(AlterColumnType::typeName())
    , m_columnName(columnName)
    , m_tableName(tableName)
    , m_newType(newType)
    , m_oldType(oldType)
{
    if(columnName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command with empty columnName!";
    }

    if(tableName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command" << columnName << "with empty tableName!";
    }

    if(newType.isEmpty()) {
        ::qWarning() << LOG_PREFIX << AlterColumnType::typeName() << "command" << columnName << "with empty newType";
    }
}

const QString &AlterColumnType::typeName()
{
    static QString sTypename("AlterColumnType");
    return sTypename;
}

const QString &AlterColumnType::tableName() const
{
    return m_tableName;
}

const QString &AlterColumnType::columnName() const
{
    return m_columnName;
}

const bool AlterColumnType::hasOldType() const
{
    return (!m_oldType.isEmpty());
}

const QString &AlterColumnType::newType() const
{
    return m_newType;
}

const QString &AlterColumnType::oldType() const
{
    return m_oldType;
}

} // namespace Commands
