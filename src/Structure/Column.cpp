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

Column::Column(const QString &name, const QString &sqlType, Attributes attributes)
    : m_defaultValue(0)
    , m_name(name)
    , m_sqlType(sqlType)
    , m_attributes(attributes)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column with empty name!";
    }

    if(sqlType.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "Column" << m_name << "with empty type!";
    }
}

Column::Column()
    : m_defaultValue(0)
    , m_name("")
    , m_sqlType("")
    , m_attributes(None)
{}

const QString &Column::name() const
{
    return m_name;
}

const QString &Column::sqlType() const
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
    return (*m_defaultValue.data());
}

bool Column::hasDefaultValue() const
{
    return (!m_defaultValue.isNull());
}

bool Column::isAutoIncremented() const
{
    return m_attributes.testFlag(AutoIncrement);
}

Column &Column::setDefault(const QString &defaultValue)
{
    m_defaultValue = QSharedPointer<QString>(new QString(defaultValue));
    return (*this);
}

} //namespace Structure
