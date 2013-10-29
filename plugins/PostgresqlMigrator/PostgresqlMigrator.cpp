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
#include "PostgresqlMigrator/PostgresqlMigrator.h"

#include "BaseSqlMigrator/CommandExecution/BaseSqlAddColumnService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlCreateIndexService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlCreateTableService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlDropColumnService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlDropIndexService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlDropTableService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlRenameTableService.h"
#include "BaseSqlMigrator/CommandExecution/baseSqlRenameColumnService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlAlterColumnTypeService.h"

#include "Helper/HelperRepository.h"

#include "PostgresqlMigrator/Helper/PostgresqlQuoteService.h"
#include "PostgresqlMigrator/Helper/PostgresqlDbReaderService.h"
#include "PostgresqlMigrator/Helper/PostgresqlColumnService.h"
#include "PostgresqlMigrator/Helper/PostgresqlTypeMapperService.h"

#include "CommandExecution/CustomCommandService.h"
#include "BaseSqlMigrator/MigrationTracker/MigrationTableService.h"
#include "MigrationExecution/MigrationExecutionContext.h"

#include <QSqlDatabase>

#include <QDebug>

namespace PostgresqlMigrator {

QSharedPointer<CommandExecution::CommandExecutionServiceRepository> createCommandServiceRepository()
{
    using namespace CommandExecution;

    QSharedPointer<CommandExecutionServiceRepository> commandRepository(new CommandExecutionServiceRepository);
    commandRepository->add(BaseCommandServicePtr(new BaseSqlAddColumnService));
    commandRepository->add(BaseCommandServicePtr(new BaseSqlAlterColumnTypeService));
    commandRepository->add(BaseCommandServicePtr(new BaseSqlCreateIndexService));
    commandRepository->add(BaseCommandServicePtr(new BaseSqlCreateTableService));
    commandRepository->add(BaseCommandServicePtr(new BaseSqlDropColumnService));
    commandRepository->add(BaseCommandServicePtr(new BaseSqlDropIndexService));
    commandRepository->add(BaseCommandServicePtr(new BaseSqlDropTableService));
    commandRepository->add(BaseCommandServicePtr(new BaseSqlRenameColumnService));
    commandRepository->add(BaseCommandServicePtr(new BaseSqlRenameTableService));
    commandRepository->add(BaseCommandServicePtr(new CustomCommandService));

    return commandRepository;
}

void createHelperRepository(Helper::HelperRepository &helperRepository)
{
    ::qDebug() << "creating PostgreSQL helper aggregate";

    using namespace Helper;

    helperRepository.columnService.reset(new PostgresqlColumnService);
    helperRepository.dbReaderService.reset(new PostgresqlDbReaderService);
    helperRepository.quoteService.reset(new PostgresqlQuoteService);
    helperRepository.typeMapperService.reset(new PostgresqlTypeMapperService);
}

bool buildContext(MigrationExecution::MigrationExecutionContext &context, QSqlDatabase database)
{
    using namespace MigrationExecution;

    CommandServiceRepositoryPtr commandRepository = createCommandServiceRepository();
    Helper::HelperRepository helperRepository;
    createHelperRepository(helperRepository);

    MigrationTableServicePtr migrationTableService =
            MigrationTableServicePtr(new MigrationTracker::MigrationTableService);

    context.setCommandServiceRepository(commandRepository);
    context.setHelperRepository(helperRepository);
    context.setBaseMigrationTableService(migrationTableService);
    context.setDatabase(database);

    bool success = false;
    if( context.database().open() ) {
        success = migrationTableService->ensureVersionTable(context);
    }
    return success;
}

} // namespace PostgresqlMigrator
