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
#ifndef STRUCTURE_TABLE_H
#define STRUCTURE_TABLE_H

#include "Structure/Column.h"

#include <QList>
#include <QString>

namespace Structure {

/*!
 * \brief The Table class represents basic table-structure.
 */
class QSQLMIGRATOR_DLL_EXPORT Table
{
public:
    explicit Table(const QString &name);
    explicit Table(const QString &name, const QList<Column> &columns);

    const QString &name() const;
    const QList<Column> &columns() const;

    const QString joinedColumnNames(const QString &delimiter = ", ") const;

    static const Table copyWithoutColumn(const Table &table, const QString &columnName);

    Table &add(const Column &column);

    inline Table &operator<<(const Column &column) {
        return this->add(column);
    }

private:
    QString m_name;
    QList<Column> m_columns;
};

} //namespace Structure

#endif // STRUCTURE_TABLE_H
