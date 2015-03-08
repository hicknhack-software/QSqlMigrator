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
#include "SqlMigrator.h"

#include "MigrationExecutor.h"

namespace QSqlMigrator {
namespace {
using MigrationExecutor = SqlDatabaseMigrator::MigrationExecutor;
}

SqlMigrator::SqlMigrator(Setup setup)
    : m_setup(std::move(setup))
    , m_definedMigrations(m_setup.migrations.migrationNames())
{
    m_definedMigrations.sort();
}

SqlMigrator::MigrationNames
SqlMigrator::appliedMigrations() const
{
    auto names = m_setup.migrationTracker->trackedNames();
    names.sort();
    return names;
}

SqlMigrator::MigrationNames
SqlMigrator::definedMigrations() const
{
    return m_definedMigrations;
}

SqlMigrator::MigrationName
SqlMigrator::lastAppliedMigration() const
{
    const auto appliedMigrations = this->appliedMigrations();
    if (appliedMigrations.isEmpty())
        return {};
    return appliedMigrations.last();
}

SqlMigrator::MigrationNames
SqlMigrator::missingMigrations() const
{
    // applied - defined
    MigrationNames missingMigrations;
    for (const auto &migrationName : this->appliedMigrations()) {
        if (!m_definedMigrations.contains(migrationName)) {
            missingMigrations << migrationName;
        }
    }
    return missingMigrations;
}

SqlMigrator::MigrationNames
SqlMigrator::unappliedMigrations() const
{
    // defined - applied
    MigrationNames unappliedMigrations;
    const auto appliedMigrations = this->appliedMigrations();
    for (const auto &migrationName : m_definedMigrations) {
        if (!appliedMigrations.contains(migrationName)) {
            unappliedMigrations << migrationName;
        }
    }
    return unappliedMigrations;
}

bool
SqlMigrator::applyAll() const
{
    const auto migrator = MigrationExecutor{ m_setup };
    return migrator.executeBatch(this->unappliedMigrations());
}

bool SqlMigrator::revertAll() const
{
    const auto migrator = MigrationExecutor{ m_setup };
    return migrator.executeBatch(this->unappliedMigrations(), MigrationExecutor::Revert);
}

bool
SqlMigrator::migrateTo(const MigrationName &migrationName) const
{
    if (!m_definedMigrations.contains(migrationName)) {
        m_setup.logging->error("migration is not defined");
        return false;
    }

    const auto appliedMigrations = this->appliedMigrations();
    auto missingMigrations = MigrationNames{};
    auto direction = MigrationExecutor::Apply;
    auto it = std::find(appliedMigrations.begin(), appliedMigrations.end(), migrationName);
    if (it != appliedMigrations.end()) {
        direction = MigrationExecutor::Revert;
        it++;
        for (; it != appliedMigrations.end(); it++)
            missingMigrations << *it;
    }
    else {
        for (const auto &defined : m_definedMigrations) {
            if (appliedMigrations.contains(defined))
                continue; // do not apply twice
            missingMigrations << defined;
            if (defined == migrationName)
                break; // done
        }
    }
    if (missingMigrations.isEmpty())
        return true; // nothing to do

    const auto migrator = MigrationExecutor{ m_setup };
    return migrator.executeBatch(missingMigrations, direction);
}

bool
SqlMigrator::applyMigration(const MigrationName &migrationName) const
{
    const auto migrator = MigrationExecutor{ m_setup };
    return migrator.execute(migrationName);
}

bool
SqlMigrator::revertMigration(const MigrationName &migrationName) const
{
    const auto migrator = MigrationExecutor{ m_setup };
    return migrator.execute(migrationName, MigrationExecutor::Revert);
}

} // namespace QSqlMigrator
