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

#include "config.h"

#include <QString>

namespace QSqlMigrator {
namespace SqlDatabaseAdapter {

/// \brief interface for a service that is used to quote different kinds of strings for a database
class QSQLMIGRATOR_DLL_EXPORT Quotation {
public:
    using Quoted = QString;
    using TableName = QString;
    using IndexName = QString;
    using ColumnName = QString;
    using ValueString = QString;

public:
    /// \return the table name properly quoted for an sql execution
    virtual Quoted quoteTableName(const TableName &) const = 0;

    /// \return the index name properly quoted for an sql execution
    virtual Quoted quoteIndexName(const IndexName &) const = 0;

    /// \return the column name properly quoted for an sql execution
    virtual Quoted quoteColumnName(const ColumnName &) const = 0;

    /// \return the string value properly quoted for an sql execution
    virtual Quoted quoteValue(const ValueString &) const = 0;
};

} // namespace SqlDatabaseAdapter
} // namespace QSqlMigrator
