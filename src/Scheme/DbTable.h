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
#ifndef SCHEME_DBTABLE_H
#define SCHEME_DBTABLE_H

#include "config.h"

#include "Scheme/DbColumn.h"

#include <QList>
#include <QString>

namespace Scheme {

class QSQLMIGRATOR_DLL_EXPORT DbTable
{
public:
    explicit DbTable(const QString &name);
    explicit DbTable(const QString &name, const QList<DbColumn> &columns);

    QList<DbColumn> &columns();
    const QList<DbColumn> &columns() const;
    const QString &name() const;
    void setName(const QString &newName);

    DbTable &add(const DbColumn &newColumn);
    DbTable &remove(const DbColumn &column) {
        return this->removeColumn(column.name());
    }
    DbTable &removeColumn(const QString &columnName);

    inline DbTable &operator<<(const DbColumn &column) {
        return this->add(column);
    }

private:
    QString m_name;
    QList<DbColumn> m_columns;
};

} // namespace Scheme

#endif // SCHEME_DBTABLE_H
