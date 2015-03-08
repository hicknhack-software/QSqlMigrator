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
#pragma once

#include "SqlDatabaseAdapter/ColumnMapper.h"

#include <QSharedPointer>

#include <functional>

namespace QSqlMigrator {
namespace SqlDatabaseAdapter {

class ValueTypeMapper;

class QSQLMIGRATOR_DLL_EXPORT StandardColumnMapper : public ColumnMapper
{
public:
    using SqlOutputFunction = std::function<void(const Sql &)>;
    using ValueTypeMapper = QSharedPointer<const ValueTypeMapper>;

public:
    StandardColumnMapper(const ValueTypeMapper&);

    /// \return the SQL type string for the column type
    virtual Sql buildColumnTypeSql(const Column &) const;

    /// \brief call the addOption callback for each additional SQL column options string
    virtual void buildColumnOptionsSql(const Column &, const SqlOutputFunction &addOption) const;

    Sql generateColumnDefinitionSql(const Column &) const Q_DECL_OVERRIDE;
    Sql generateColumnsDefinitionSql(const Columns &) const Q_DECL_OVERRIDE;

    Sql generateIndexColumnDefinitionSql(const IndexColumn &) const Q_DECL_OVERRIDE;
    Sql generateIndexColumnsDefinitionSql(const IndexColumns &) const Q_DECL_OVERRIDE;

protected:
    ValueTypeMapper m_valueTypeMapper;
};

} // namespace Helper
} // namespace QSqlMigrator
