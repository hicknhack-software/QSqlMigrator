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
#include "SqliteMigrator/SqliteMigrator.h"

#include "SqliteMigrator/CommandExecution/SqliteAddColumnService.h"
#include "SqliteMigrator/CommandExecution/SqliteAlterColumnTypeService.h"
#include "SqliteMigrator/CommandExecution/SqliteCreateIndexService.h"
#include "SqliteMigrator/CommandExecution/SqliteCreateTableService.h"
#include "SqliteMigrator/CommandExecution/SqliteDropColumnService.h"
#include "SqliteMigrator/CommandExecution/SqliteDropIndexService.h"
#include "SqliteMigrator/CommandExecution/SqliteDropTableService.h"
#include "SqliteMigrator/CommandExecution/SqliteRenameColumnService.h"
#include "SqliteMigrator/CommandExecution/SqliteRenameTableService.h"

#include "CommandExecution/CustomCommandService.h"
#include "MigrationExecution/MigrationExecutionContext.h"
#include "MigrationTracker/SqliteMigrationTableService.h"

#include <QSqlDatabase>

namespace SqliteMigrator {

using namespace CommandExecution;

QSharedPointer<CommandExecutionServiceRepository> commandServiceRepository()
{
    QSharedPointer<CommandExecutionServiceRepository> commandRepository(new CommandExecutionServiceRepository);
    commandRepository->add(BaseCommandServicePtr(new SqliteAddColumnService));
    commandRepository->add(BaseCommandServicePtr(new SqliteAlterColumnTypeService));
    commandRepository->add(BaseCommandServicePtr(new SqliteCreateIndexService));
    commandRepository->add(BaseCommandServicePtr(new SqliteCreateTableService));
    commandRepository->add(BaseCommandServicePtr(new SqliteDropColumnService));
    commandRepository->add(BaseCommandServicePtr(new SqliteDropIndexService));
    commandRepository->add(BaseCommandServicePtr(new SqliteDropTableService));
    commandRepository->add(BaseCommandServicePtr(new SqliteRenameColumnService));
    commandRepository->add(BaseCommandServicePtr(new SqliteRenameTableService));
    commandRepository->add(BaseCommandServicePtr(new CustomCommandService));

    return commandRepository;
}

using namespace MigrationExecution;

bool buildContext(MigrationExecutionContext &context, QSqlDatabase database)
{
    CommandServiceRepositoryPtr commandRepository = SqliteMigrator::commandServiceRepository();
    MigrationTableServicePtr migrationTableService(new MigrationTracker::SqliteMigrationTableService);

    context.setCommandServiceRepository(commandRepository);
    context.setBaseMigrationTableService(migrationTableService);
    context.setDatabase(database);

    bool success = false;
    if( context.database().open() ) {
        success = migrationTableService->ensureVersionTable(context);
    }

    return success;
}

} // namespace SqliteMigrator
