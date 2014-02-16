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

#include "Structure/Type.h"

#include <QString>
#include <QFlags>

namespace Structure {

/*!
 * \brief value object representing the column-structure.
 *
 * Columns are default NULL (as in SQLite, MySQL and PostgreSQL).
 * According to the SQL standard, PRIMARY KEY should always imply NOT NULL.
 * UNIQUE columns can contain several NULL values (as in SQLite, MySQL and PostgreSQL)
 * AutoIncrement is special: every DBMS handles some sort auf auto increment in it's own way.
 */
class QSQLMIGRATOR_DLL_EXPORT Column
{
public:
    enum Attribute {
        None = 0,
        NotNullable = (1 << 0),
        Unique = (1 << 1),
        Primary = (1 << 2) | NotNullable | Unique,
        AutoIncrement = (1 << 3)
    };
    typedef QFlags<Attribute> Attributes;

public:    
    explicit Column(const QString &name, const Type &type, const QString& defaultValue, Attributes attributes = None);
    explicit Column(const QString &name, const Type &type, Attributes attributes = None);

    const QString &name() const;
    const Type &type() const;
    const QString &defaultValue() const;
    const Attributes &attributes() const;

    bool isValid() const;
    bool hasDefaultValue() const;

    bool isNullable() const;
    bool isPrimary() const;
    bool isUnique() const;
    bool isAutoIncremented() const;

private:
    const QString m_name;
    const Type m_type;
    const QString m_defaultValue;
    const Attributes m_attributes;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Column::Attributes)

} //namespace Structure

#endif // STRUCTURE_COLUMN_H
