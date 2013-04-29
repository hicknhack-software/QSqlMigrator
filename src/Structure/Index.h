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
#ifndef STRUCTURE_INDEX_H
#define STRUCTURE_INDEX_H

#include "config.h"

#include <QHash>
#include <QString>

namespace Structure {

/*!
 * \brief The Index class represents a basic index-structure.
 */
class QSQLMIGRATOR_DLL_EXPORT Index
{
public:
    enum SortOrder{
        Default = 0,
        Ascending = 1,
        Descending = 2
    };
    typedef QHash<QString, SortOrder> IndexHash;

    explicit Index(const QString &name, const QString &tableName);
    explicit Index(const QString &name, const QString &tableName, const QStringList &columnNames);

    const IndexHash &columns() const;
    const QString &name() const;
    const QString &tableName() const;

    Index &addColumn(const QString &columnName, SortOrder sortOrder=Default);

private:
    QString m_name;
    QString m_tableName;
    IndexHash m_columns;
};

} //namespace Structure

#endif // STRUCTURE_INDEX_H
