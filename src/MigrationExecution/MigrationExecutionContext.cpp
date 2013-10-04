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
#include "MigrationExecutionContext.h"
#include "Migrations/Migration.h"
#include "MigrationTracker/BaseMigrationTrackerService.h"
#include "CommandExecution/CommandExecutionServiceRepository.h"

#include <QSqlDatabase>

namespace MigrationExecution {

MigrationExecutionContext::MigrationExecutionContext(const NameMigrationMap &migrations, const MigrationExecutionConfig &migrationConfig)
    : m_migrations(migrations)
    , m_migrationConfig(migrationConfig)
{
}

MigrationExecutionContext::MigrationExecutionContext(const MigrationExecutionContext::NameMigrationMap &migrations)
    : m_migrations(migrations)
{
}

QSqlDatabase MigrationExecutionContext::database() const
{
    return m_database;
}

const MigrationExecutionConfig &MigrationExecutionContext::migrationConfig() const
{
    return m_migrationConfig;
}

const MigrationExecutionContext::NameMigrationMap &MigrationExecutionContext::migrationMap() const
{
    return m_migrations;
}

CommandServiceRepositoryPtr MigrationExecutionContext::commandServiceRepository() const
{
    return m_commandServiceRepository;
}

MigrationTableServicePtr MigrationExecutionContext::baseMigrationTableService() const
{
    return m_migrationTableService;
}

void MigrationExecutionContext::setDatabase(QSqlDatabase database)
{
    m_database = database;
}

void MigrationExecutionContext::setCommandServiceRepository(CommandServiceRepositoryPtr commandServiceRepository)
{
    m_commandServiceRepository = commandServiceRepository;
}

void MigrationExecutionContext::setBaseMigrationTableService(MigrationTableServicePtr baseMigrationTableService)
{
    m_migrationTableService = baseMigrationTableService;
}

} // namespace MigrationExecution
