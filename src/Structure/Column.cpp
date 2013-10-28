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
#include "Column.h"

#include <QDebug>

namespace Structure {

Column::Column(const QString &name, const QString &sqlTypeString, const QString &defaultValue, Attributes attributes)
    : m_name(name)
    , m_sqlTypeString(sqlTypeString)
    , m_defaultValue(defaultValue)
    , m_attributes(attributes)
    , m_hasSqlTypeString(true)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column with empty name!";
    }

    if(sqlTypeString.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column" << m_name << "with empty type!";
    }
}

Column::Column(const QString &name, const SqlType &sqltype, const QString &defaultValue, Attributes attributes)
    : m_name(name)
    , m_sqlType(sqltype)
    , m_defaultValue(defaultValue)
    , m_attributes(attributes)
    , m_hasSqlTypeString(false)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column with empty name!";
    }
}

Column::Column(const QString &name, const QString &sqlTypeString, Attributes attributes)
    : m_name(name)
    , m_sqlTypeString(sqlTypeString)
    , m_defaultValue("")
    , m_attributes(attributes)
    , m_hasSqlTypeString(true)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column with empty name!";
    }

    if(sqlTypeString.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column" << m_name << "with empty type!";
    }
}

Column::Column(const QString &name, const SqlType &sqltype, Attributes attributes)
    : m_name(name)
    , m_sqlType(sqltype)
    , m_defaultValue("")
    , m_attributes(attributes)
    , m_hasSqlTypeString(false)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column with empty name!";
    }
}

Column::Column()
    : m_name("")
    , m_sqlTypeString("")
    , m_defaultValue("")
    , m_attributes(None)
    , m_hasSqlTypeString(false)
{}

const QString &Column::name() const
{
    return m_name;
}

const QString &Column::sqlTypeString() const
{
    return m_sqlTypeString;
}

const SqlType &Column::sqlType() const
{
    return m_sqlType;
}

const Column::Attributes &Column::attributes() const
{
    return m_attributes;
}

bool Column::isNullable() const
{
    return !m_attributes.testFlag(NotNullable);
}

bool Column::isPrimary() const
{
    return m_attributes.testFlag(Primary);
}

bool Column::isUnique() const
{
    return m_attributes.testFlag(Unique);
}

const QString &Column::defaultValue() const
{
    return (m_defaultValue);
}

bool Column::hasDefaultValue() const
{
    return (!m_defaultValue.isEmpty());
}

bool Column::hasSqlTypeString() const
{
   return m_hasSqlTypeString;
}

bool Column::isAutoIncremented() const
{
    return m_attributes.testFlag(AutoIncrement);
}

} //namespace Structure
