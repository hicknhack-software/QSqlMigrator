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

#include "LoggingTrace/Logging.h"
#include "SqlSchema/Schema.h"
#include "SqlDatabaseAdapter/Adapter.h"

#include "config.h"

#include <QSqlDatabase>
#include <QSharedPointer>

namespace QSqlMigrator {
namespace SqlDatabaseSchemaValidator {

class QSQLMIGRATOR_DLL_EXPORT DatabaseSchemaComparison
{
public:
    using Adapter = SqlDatabaseAdapter::Adapter;
    using Schema = SqlSchema::Schema;
    using Reporter = QSharedPointer<LoggingTrace::Logging>;

public:
    /// \return true if all schema parts are present in the database otherwise report missing parts as errors
    bool reportMissingSchemaParts(const Schema&, const Adapter&, const Reporter &) const;
};

} // namespace SqlDatabaseSchemaValidator
} // namespace QSqlMigrator
