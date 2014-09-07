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
#include "FirebirdMigrator/FirebirdMigrator.h"

#include "Helper/HelperRepository.h"

#include "BaseSqlMigrator/Helper/BaseSqlQuoteService.h"
#include "BaseSqlMigrator/Helper/BaseSqlColumnService.h"
#include "FirebirdMigrator/Helper/FirebirdSqlStructureService.h"
#include "BaseSqlMigrator/Helper/BaseSqlTypeMapperService.h"
#include "MigrationTracker/MigrationTrackerService.h"

#include "CommandExecution/CustomCommandService.h"
#include "BaseSqlMigrator/MigrationTracker/BaseMigrationTableService.h"
#include "MigrationExecution/MigrationExecutionContext.h"

#include <QSqlDatabase>

#include <QDebug>

namespace FirebirdMigrator {

QSharedPointer<CommandExecution::CommandExecutionServiceRepository> createCommandServiceRepository()
{
    using namespace CommandExecution;

    QSharedPointer<CommandExecutionServiceRepository> commandRepository(new CommandExecutionServiceRepository);
    //TODO KP: Commands for Firebird to add

    return commandRepository;
}

Helper::HelperRepository &createHelperRepository()
{
    ::qDebug() << "creating Firebird helper aggregate";

    using namespace Helper;

    static BaseSqlQuoteService quoteService;
    static BaseSqlTypeMapperService typeMapperService;
    static BaseSqlColumnService columnService(typeMapperService);
    static FirebirdSqlStructureService structureService;
    static HelperRepository repository(quoteService, typeMapperService, columnService, structureService);

    return repository;
}

MigrationExecution::MigrationExecutionContextPtr buildContext(MigrationExecution::MigrationExecutionContext::Builder &contextBuilder)
{
    using namespace MigrationExecution;

    CommandServiceRepositoryPtr commandRepository(createCommandServiceRepository());
    MigrationTableServicePtr migrationTableService(new MigrationTracker::BaseMigrationTableService);

    MigrationExecutionContextPtr context = contextBuilder.build(commandRepository, createHelperRepository(), migrationTableService);

    QSqlDatabase database(context->database());
    bool success = false;
    if (database.open()) {
        success = migrationTableService->prepare(*context);
    }
    if (!success )
        context.clear();

    return context;
}

} // namespace FirebirdMigrator
