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

Column::Column(const QString &name, const Type &type, const QString &defaultValue, Attributes attributes)
    : m_name(name)
    , m_type(type)
    , m_defaultValue(defaultValue)
    , m_attributes(attributes)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column with empty name!";
    }
}

Column::Column(const QString &name, const Type &type, Attributes attributes)
    : m_name(name)
    , m_type(type)
    , m_attributes(attributes)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column with empty name!";
    }
}

const QString &Column::name() const
{
    return m_name;
}

const Type &Column::type() const
{
    return m_type;
}

const QString &Column::defaultValue() const
{
    return m_defaultValue;
}

const Column::Attributes &Column::attributes() const
{
    return m_attributes;
}

bool Column::isValid() const
{
    return (!m_name.isEmpty()) && m_type.isValid();
}

bool Column::hasDefaultValue() const
{
    return (!m_defaultValue.isEmpty());
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

bool Column::isAutoIncremented() const
{
    return m_attributes.testFlag(AutoIncrement);
}

} //namespace Structure
