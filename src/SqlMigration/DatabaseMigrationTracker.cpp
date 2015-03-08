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
#include "DatabaseMigrationTracker.h"

#include "SqlDatabaseAdapter/Quotation.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

namespace QSqlMigrator {
namespace SqlMigration {

DatabaseMigrationTracker::DatabaseMigrationTracker(Adapter adapter, Logging logging, Config config)
    : m_adapter(std::move(adapter))
    , m_logging(std::move(logging))
    , m_config(std::move(config))
{
}

bool
DatabaseMigrationTracker::isPrepared() const
{
    if (!m_adapter.database().tables().contains(m_config.tableName))
        return false; // tables does not exist
    return true;
}

bool
DatabaseMigrationTracker::prepare() const
{
    const auto scope = m_logging->scope([]() { return Q_FUNC_INFO; });
    Q_UNUSED(scope);

    const auto quotation = m_adapter.quotation();
    const auto tableName = m_config.tableName;
    const auto query = QString("CREATE TABLE %1 (version VARCHAR(255) NOT NULL, PRIMARY KEY (version))")
                           .arg(quotation->quoteTableName(tableName));
    const auto queryScope = m_logging->scope([&]() { return query; });
    Q_UNUSED(queryScope);

    auto sqlQuery = m_adapter.database().exec(query);
    if (sqlQuery.lastError().isValid()) {
        m_logging->error(sqlQuery.lastError().text());
        return false;
    }
    return true;
}

MigrationTracker::MigrationNames
DatabaseMigrationTracker::trackedNames() const
{
    const auto scope = m_logging->scope([]() { return Q_FUNC_INFO; });
    Q_UNUSED(scope);

    const auto quotation = m_adapter.quotation();
    const auto tableName = m_config.tableName;
    const auto query = QString("SELECT version FROM %1").arg(quotation->quoteTableName(tableName));
    const auto queryScope = m_logging->scope([&]() { return query; });
    Q_UNUSED(queryScope);

    auto sqlQuery = m_adapter.database().exec(query);
    if (sqlQuery.lastError().isValid()) {
        m_logging->error(sqlQuery.lastError().text());
        return {};
    }
    QStringList migrationNames;
    while (sqlQuery.next()) {
        migrationNames << sqlQuery.value(0).toString();
    }
    return migrationNames;
}

bool
DatabaseMigrationTracker::wasTracked(const MigrationTracker::MigrationName &migrationName) const
{
    return this->trackedNames().contains(migrationName);
}

bool
DatabaseMigrationTracker::track(const MigrationTracker::MigrationName &migrationName) const
{
    const auto scope = m_logging->scope([]() { return Q_FUNC_INFO; });
    Q_UNUSED(scope);

    const auto quotation = m_adapter.quotation();
    const auto tableName = m_config.tableName;
    const auto query = QString("INSERT INTO %1 (version) VALUES (%2)")
                           .arg(quotation->quoteTableName(tableName))
                           .arg(quotation->quoteValue(migrationName));
    const auto queryScope = m_logging->scope([&]() { return query; });
    Q_UNUSED(queryScope);

    auto sqlQuery = m_adapter.database().exec(query);
    if (sqlQuery.lastError().isValid()) {
        m_logging->error(sqlQuery.lastError().text());
        return false;
    }
    return true;
}

bool
DatabaseMigrationTracker::forget(const MigrationTracker::MigrationName &migrationName) const
{
    const auto scope = m_logging->scope([]() { return Q_FUNC_INFO; });
    Q_UNUSED(scope);

    const auto quotation = m_adapter.quotation();
    const auto tableName = m_config.tableName;
    const auto query = QString("DELETE FROM %1 WHERE version = %2")
                           .arg(quotation->quoteTableName(tableName))
                           .arg(quotation->quoteValue(migrationName));
    const auto queryScope = m_logging->scope([&]() { return query; });
    Q_UNUSED(queryScope);

    auto sqlQuery = m_adapter.database().exec(query);
    if (sqlQuery.lastError().isValid()) {
        m_logging->error(sqlQuery.lastError().text());
        return false;
    }
    return true;
}

} // namespace SqlMigration
} // namespace QSqlMigrator
