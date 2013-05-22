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
#include "DbColumn.h"

namespace Scheme {

DbColumn::DbColumn(const QString &name, const QString &sqlType, Attributes attributes)
    : m_name(name)
    , m_sqlType(sqlType)
    , m_attributes(attributes)
{
}

QString &DbColumn::name()
{
    return const_cast<QString &>(static_cast<const DbColumn &>(*this).name());
}

const QString &DbColumn::name() const
{
    return m_name;
}

QString &DbColumn::sqlType()
{
    return const_cast<QString &>(static_cast<const DbColumn &>(*this).sqlType());
}

const QString &DbColumn::sqlType() const
{
    return m_sqlType;
}

DbColumn::Attributes &DbColumn::attributes()
{
    return const_cast<Attributes &>(static_cast<const DbColumn &>(*this).attributes());
}

const DbColumn::Attributes &DbColumn::attributes() const
{
    return m_attributes;
}

} // namespace Scheme
