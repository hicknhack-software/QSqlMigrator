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
#ifndef SCHEME_DBCOLUMN_H
#define SCHEME_DBCOLUMN_H

#include "config.h"

#include <QFlags>
#include <QString>

namespace Scheme {

class QSQLMIGRATOR_DLL_EXPORT DbColumn
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
    explicit DbColumn(const QString &name, const QString &sqlType, Attributes attributes = Nullable);

    QString &name();
    const QString &name() const;

    QString &sqlType();
    const QString &sqlType() const;

    Attributes &attributes();
    const Attributes &attributes() const;

private:
    QString m_name;
    QString m_sqlType;
    Attributes m_attributes;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DbColumn::Attributes)

} // namespace Scheme

#endif // SCHEME_DBCOLUMN_H
