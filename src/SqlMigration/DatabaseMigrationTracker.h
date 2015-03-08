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

#include "SqlMigration/MigrationTracker.h"
#include "SqlDatabaseAdapter/Adapter.h"
#include "LoggingTrace/Logging.h"

#include "config.h"

#include <QSharedPointer>

namespace QSqlMigrator {
namespace SqlMigration {

/// \brief Implementation of the successfully executed migration tracking through an sql databse table
class QSQLMIGRATOR_DLL_EXPORT DatabaseMigrationTracker : public MigrationTracker {
public:
    using Adapter = SqlDatabaseAdapter::Adapter;
    using Logging = QSharedPointer<LoggingTrace::Logging>;
    struct Config {
        QString tableName = "qt_tracked_migrations"; ///< name of the version table
    };

public:
    DatabaseMigrationTracker(Adapter, Logging, Config = {});

    // MigrationTracker interface
    bool isPrepared() const Q_DECL_OVERRIDE;
    bool prepare() const Q_DECL_OVERRIDE;
    MigrationNames trackedNames() const Q_DECL_OVERRIDE;
    bool wasTracked(const MigrationName &) const Q_DECL_OVERRIDE;
    bool track(const MigrationName &) const Q_DECL_OVERRIDE;
    bool forget(const MigrationName &) const Q_DECL_OVERRIDE;

private:
    Adapter m_adapter;
    Logging m_logging;
    Config m_config;
};

} // namespace SqlMigration
} // namespace QSqlMigrator
