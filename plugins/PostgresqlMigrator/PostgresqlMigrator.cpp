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
#include "BaseSqlMigrator/CommandExecution/BaseSqlRenameColumnService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlAlterColumnTypeService.h"

#include "Helper/HelperRepository.h"

#include "PostgresqlMigrator/Helper/PostgresqlQuoteService.h"
#include "PostgresqlMigrator/Helper/PostgresqlStructureService.h"
#include "PostgresqlMigrator/Helper/PostgresqlColumnService.h"
#include "PostgresqlMigrator/Helper/PostgresqlTypeMapperService.h"

#include "CommandExecution/CustomCommandService.h"
#include "BaseSqlMigrator/MigrationTracker/BaseMigrationTableService.h"
#include "MigrationExecution/MigrationExecutionContext.h"

#include <QSqlDatabase>

#include <QDebug>

namespace PostgresqlMigrator {

QSharedPointer<CommandExecution::CommandExecutionServiceRepository> createCommandRepository()
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

Helper::HelperRepository &createHelperRepository()
{
    ::qDebug() << "creating PostgreSQL helper aggregate";

    using namespace Helper;
    static PostgresqlQuoteService quoteService;
    static PostgresqlTypeMapperService typeMapperService;
    static PostgresqlColumnService columnService(typeMapperService);
    static PostgresqlStructureService structureService;
    static HelperRepository repository(quoteService, typeMapperService, columnService, structureService);
    return repository;
}

MigrationExecution::MigrationExecutionContextPtr buildContext(MigrationExecution::MigrationExecutionContext::Builder &contextBuilder)
{
    using namespace MigrationExecution;

    MigrationTrackerServicePtr migrationTableService(new MigrationTracker::BaseMigrationTableService);

    MigrationExecution::MigrationExecutionContextPtr context( contextBuilder.build(createCommandRepository(), createHelperRepository(), migrationTableService) );

    QSqlDatabase database(context->database());
    bool success = false;
    if (database.open()) {
        success = migrationTableService->prepare(*context);
    }
    if (!success )
        context.clear();

    return context;
}

} // namespace PostgresqlMigrator
