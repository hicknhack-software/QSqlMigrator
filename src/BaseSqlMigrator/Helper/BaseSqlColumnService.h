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
#ifndef HELPER_BASESQLCOLUMNSERVICE_H
#define HELPER_BASESQLCOLUMNSERVICE_H

#include "Structure/Column.h"
#include "Structure/Index.h"

#include "Helper/ColumnService.h"
#include "Helper/TypeMapperService.h"

#include <QString>
#include <QHash>

namespace Helper {

class QSQLMIGRATOR_DLL_EXPORT BaseSqlColumnService : public ColumnService
{
public:
    BaseSqlColumnService(const TypeMapperService& typeMapperService);
    ~BaseSqlColumnService() QSQL_OVERRIDE_D {}

    virtual QStringList buildColumnOptionsSql(const Structure::Column &column) const;

    QString generateColumnDefinitionSql(const Structure::Column &column) const Q_DECL_OVERRIDE;
    QString generateColumnsDefinitionSql(const QList<Structure::Column> &columnList) const Q_DECL_OVERRIDE;

    QString generateIndexColumnDefinitionSql(const Structure::Index::Column &column) const Q_DECL_OVERRIDE;
    QString generateIndexColumnsDefinitionSql(const Structure::Index::ColumnList &columns) const Q_DECL_OVERRIDE;

protected:
    const TypeMapperService& m_typeMapperService;
};

} // namespace Helper

#endif // HELPER_BASESQLCOLUMNSERVICE_H
