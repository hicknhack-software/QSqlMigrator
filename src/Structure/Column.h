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
#ifndef STRUCTURE_COLUMN_H
#define STRUCTURE_COLUMN_H

#include "config.h"

#include <QSharedPointer>
#include <QString>
#include <QFlags>

namespace Structure {

/*!
 * \brief The Column class represents a basic column-structure.
 */
class QSQLMIGRATOR_DLL_EXPORT Column
{
public:
    enum Attribute {
        None = 0,
        NotNullable = 0,
        Nullable = (1 << 0),
        Unique = (1 << 1),
        Primary = (1 << 2),
        AutoIncrement = (1 << 3)
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

public:
    explicit Column(const QString &name, const QString &sqlType, Attributes attributes = Nullable);

    bool isNullable() const;
    bool isPrimary() const;
    bool isUnique() const;
    bool hasDefaultValue() const;
    const Attributes &attributes() const;
    const QString &defaultValue() const;
    const QString &name() const;
    const QString &sqlType() const;

    Column &setDefault(const QString &defaultValue);

private:
    QSharedPointer<QString> m_defaultValue;
    QString m_name;
    QString m_sqlType;
    /* const */ Attributes m_attributes;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Column::Attributes)

} //namespace Structure

#endif // STRUCTURE_COLUMN_H
