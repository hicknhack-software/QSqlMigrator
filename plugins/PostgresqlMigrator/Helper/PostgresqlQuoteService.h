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

#include "PostgresqlMigrator/PostgresqlMigrator.h"

#include "PostgresqlMigrator/Helper/PostgresqlQuoteService.h"

#include "BaseSqlMigrator/Helper/BaseSqlQuoteService.h"

#include <QString>

namespace Helper {

class POSTGRESQLMIGRATOR_DLL_EXPORT PostgresqlQuoteService : public BaseSqlQuoteService
{
public:
    PostgresqlQuoteService();
    ~PostgresqlQuoteService();

    QString quoteTableName(const QString &tableName) const Q_DECL_OVERRIDE;
    QString quoteColumnName(const QString &columnName) const Q_DECL_OVERRIDE;
    QString quoteValue(const QString &string) const Q_DECL_OVERRIDE;

    QString quoteUnicodeString(const QString &string, const char UESCAPE = '\\') const;
};

} // namespace Helper
