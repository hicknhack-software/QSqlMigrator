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
#ifndef HELPER_MYSQLCOLUMNSERVICE_H
#define HELPER_MYSQLCOLUMNSERVICE_H

#include "Structure/Column.h"
#include "Structure/Index.h"

#include <QString>

namespace Helper {

class MysqlColumnService
{
public:
    MysqlColumnService();

    QString generateColumnDefinitionSql(const Structure::Column &column) const;
    QString generateColumnDefinitionSql(const QList<Structure::Column> &columnList) const;
    QString generateIndexColumnDefinitionSql(const QString &columnName
                                                   , const Structure::Index::SortOrder &sortOrder) const;
    QString generateIndexColumnDefinitionSql(const Structure::Index::IndexHash &columns) const;
};

} // namespace Helper

#endif // HELPER_MYSQLCOLUMNSERVICE_H
