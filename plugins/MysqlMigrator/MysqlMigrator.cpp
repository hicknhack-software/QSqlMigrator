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
#include "BaseSqlMigrator/BaseSqlMigrator.h"
#include "MysqlMigrator/MysqlMigrator.h"

#include "CommandExecution/CustomCommandService.h"

#include "BaseSqlMigrator/MigrationTracker/MigrationTableService.h"
#include "MigrationExecution/MigrationExecutionContext.h"


namespace MysqlMigrator {

bool buildContext(MigrationExecution::MigrationExecutionContext &context, QSqlDatabase database)
{
    using namespace MigrationExecution;

    CommandServiceRepositoryPtr commandRepository = BaseSqlMigrator::commandServiceRepository();
    Helper::HelperAggregate helperAggregate;
    BaseSqlMigrator::createHelperAggregate(helperAggregate);

    MigrationTableServicePtr migrationTableService =
            MigrationTableServicePtr(new MigrationTracker::MigrationTableService);

    context.setCommandServiceRepository(commandRepository);
    context.setHelperAggregate(helperAggregate);
    context.setBaseMigrationTableService(migrationTableService);
    context.setDatabase(database);

    bool success = false;
    if( context.database().open() ) {
        success = migrationTableService->ensureVersionTable(context);
    }
    return success;
}

} // namespace MysqlMigrator
