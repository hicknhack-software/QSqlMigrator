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
#include <QVariant>

// available in global namespace
class sqlType {
public:
    sqlType() {}
    sqlType(QVariant::Type t, int precision=0, int scale=0) : type(t), n(precision), m(scale) {}
    QVariant::Type type;
    int n;
    int m;
};

namespace Structure {

/*!
 * \brief The Column class represents a basic column-structure.
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
        //Nullable = 0,
        NotNullable = (1 << 0),
        Unique = (1 << 1),
        Primary = (1 << 2) | (1 << 0),
        AutoIncrement = (1 << 3)
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

public:
    explicit Column(const QString &name, const QString &sqlTypeString, Attributes attributes = None);
    explicit Column(const QString &name, const sqlType &sqltype, Attributes attributes = None);
    Column(); // empty columns are needed at several places and using the constructor above generates warnings (ugly)

    bool isNullable() const;
    bool isPrimary() const;
    bool isUnique() const;
    bool hasDefaultValue() const;
    bool hasSqlTypeString() const;
    bool isAutoIncremented() const;
    const Attributes &attributes() const;
    const QString &defaultValue() const;
    const QString &name() const;
    const QString &sqlTypeString() const;
    const ::sqlType &sqlType() const;

    Column &setDefault(const QString &defaultValue);

private:
    QSharedPointer<QString> m_defaultValue;
    QString m_name;
    bool m_hasSqlTypeString;
    ::sqlType m_sqlType;
    QString m_sqlTypeString;
    /* const */ Attributes m_attributes;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Column::Attributes)

} //namespace Structure

#endif // STRUCTURE_COLUMN_H
