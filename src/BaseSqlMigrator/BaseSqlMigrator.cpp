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

#include "BaseSqlMigrator/CommandExecution/BaseSqlAddColumnService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlAlterColumnTypeService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlCreateIndexService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlCreateTableService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlDropColumnService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlDropIndexService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlDropTableService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlRenameColumnService.h"
#include "BaseSqlMigrator/CommandExecution/BaseSqlRenameTableService.h"

#include "BaseSqlMigrator/Helper/BaseSqlColumnService.h"
#include "BaseSqlMigrator/Helper/BaseSqlDbReaderService.h"
#include "BaseSqlMigrator/Helper/BaseSqlQuoteService.h"

#include "CommandExecution/CustomCommandService.h"
#include "MigrationExecution/MigrationExecutionContext.h"

#include <QSqlDatabase>

#include <QDebug>

namespace BaseSqlMigrator {

QSharedPointer<CommandExecution::CommandExecutionServiceRepository> commandServiceRepository()
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

void createHelperAggregate(Helper::HelperAggregate &helperAggregate)
{
    ::qDebug() << "creating BaseSql helper aggregate";

    helperAggregate.columnService.reset(new Helper::BaseSqlColumnService);
    helperAggregate.dbReaderService.reset(new Helper::BaseSqlDbReaderService);
    helperAggregate.quoteService.reset(new Helper::BaseSqlQuoteService);
}


} // namespace BaseSqlMigrator
