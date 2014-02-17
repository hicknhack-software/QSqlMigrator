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
#include "MigrationTracker/MigrationTrackerService.h"

namespace MigrationExecution {

MigrationExecutionContextPtr MigrationExecutionContext::Builder::build(const CommandServiceRepositoryPtr &commandServiceRepository,
                                                                       const Helper::HelperRepository &helperRepository,
                                                                       const MigrationTableServicePtr &migrationTableService) const
{
    return MigrationExecutionContextPtr(new MigrationExecutionContext(m_migrations,
                                                                      m_migrationConfig,
                                                                      m_database,
                                                                      commandServiceRepository,
                                                                      helperRepository,
                                                                      migrationTableService));
}

MigrationExecutionContext::MigrationExecutionContext(const MigrationExecutionContext::NameMigrationMap &migrations,
                                                     const MigrationExecutionConfig &migrationConfig,
                                                     const QSqlDatabase &database,
                                                     const CommandServiceRepositoryPtr &commandServiceRepository,
                                                     const Helper::HelperRepository &helperRepository,
                                                     const MigrationTableServicePtr &migrationTableService)
    : m_migrations(migrations)
    , m_migrationConfig(migrationConfig)
    , m_database(database)
    , m_commandServiceRepository(commandServiceRepository)
    , m_helperRepository(helperRepository)
    , m_migrationTableService(migrationTableService)
{
}

} // namespace MigrationExecution
