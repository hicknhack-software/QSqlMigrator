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
#ifndef MIGRATIONEXECUTION_MIGRATIONEXECUTIONCONTEXT_H
#define MIGRATIONEXECUTION_MIGRATIONEXECUTIONCONTEXT_H

#include "CommandExecution/CommandExecutionServiceRepository.h"

#include "MigrationExecution/MigrationExecutionConfig.h"

#include "Helper/HelperRepository.h"

#include <QMap>
#include <QSharedPointer>
#include <QSqlDatabase>

class QSqlDatabase;

namespace Helper {
class HelperRepository;
} // namespace Helper
namespace Migrations {
class Migration;
} // namespace Migrations
namespace MigrationTracker {
class MigrationTrackerService;
} // namespace MigrationTracker

namespace MigrationExecution {

typedef QSharedPointer<CommandExecution::CommandExecutionServiceRepository> CommandServiceRepositoryPtr;
typedef QSharedPointer<MigrationTracker::MigrationTrackerService> MigrationTrackerServicePtr;

class MigrationExecutionContext;
typedef QSharedPointer<MigrationExecutionContext> MigrationExecutionContextPtr;

class QSQLMIGRATOR_DLL_EXPORT MigrationExecutionContext
{
public:
    typedef QMap<QString, const Migrations::Migration*> NameMigrationMap;

    class QSQLMIGRATOR_DLL_EXPORT Builder
    {
    public:
        explicit Builder(const NameMigrationMap &migrations);

        void setConfig(const MigrationExecutionConfig &migrationConfig);
        void setDatabase(const QSqlDatabase &database);

        MigrationExecutionContextPtr build(const CommandServiceRepositoryPtr &commandServiceRepository,
                                           const Helper::HelperRepository &helperRepository,
                                           const MigrationTrackerServicePtr &migrationTrackerService) const;

    private:
        const NameMigrationMap m_migrations;
        MigrationExecutionConfig m_migrationConfig;
        QSqlDatabase m_database;
    };

    MigrationExecutionContext(const NameMigrationMap &migrations,
                              const MigrationExecutionConfig &migrationConfig,
                              const QSqlDatabase &database,
                              const CommandServiceRepositoryPtr &commandServiceRepository,
                              const Helper::HelperRepository &helperRepository,
                              const MigrationTrackerServicePtr &migrationTableService);

    const MigrationTrackerServicePtr migrationTrackerService() const;
    const CommandServiceRepositoryPtr commandServiceRepository() const;
    const Helper::HelperRepository &helperRepository() const;
    const QSqlDatabase &database() const;
    const MigrationExecutionConfig &migrationConfig() const;
    const NameMigrationMap &migrationMap() const;

private:
    const NameMigrationMap m_migrations;
    const MigrationExecutionConfig m_migrationConfig;
    const QSqlDatabase m_database;
    const CommandServiceRepositoryPtr m_commandServiceRepository;
    const Helper::HelperRepository m_helperRepository;
    const MigrationTrackerServicePtr m_migrationTrackerService;
};

inline MigrationExecutionContext::Builder::Builder(const MigrationExecutionContext::NameMigrationMap &migrations)
    : m_migrations(migrations)
{
}

inline void MigrationExecutionContext::Builder::setConfig(const MigrationExecutionConfig &migrationConfig)
{
    m_migrationConfig = migrationConfig;
}

inline void MigrationExecutionContext::Builder::setDatabase(const QSqlDatabase &database)
{
    m_database = database;
}

inline const MigrationTrackerServicePtr MigrationExecutionContext::migrationTrackerService() const
{
    return m_migrationTrackerService;
}

inline const CommandServiceRepositoryPtr MigrationExecutionContext::commandServiceRepository() const
{
    return m_commandServiceRepository;
}

inline const Helper::HelperRepository &MigrationExecutionContext::helperRepository() const
{
    return m_helperRepository;
}

inline const QSqlDatabase &MigrationExecutionContext::database() const
{
    return m_database;
}

inline const MigrationExecutionConfig &MigrationExecutionContext::migrationConfig() const
{
    return m_migrationConfig;
}

inline const MigrationExecutionContext::NameMigrationMap &MigrationExecutionContext::migrationMap() const
{
    return m_migrations;
}

} // namespace MigrationExecution

#endif // MIGRATIONEXECUTION_MIGRATIONEXECUTIONCONTEXT_H
