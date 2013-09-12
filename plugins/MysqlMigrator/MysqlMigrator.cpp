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
#include "MysqlMigrator/MysqlMigrator.h"

#include "MysqlMigrator/CommandExecution/MysqlAddColumnService.h"
#include "MysqlMigrator/CommandExecution/MysqlAlterColumnTypeService.h"
#include "MysqlMigrator/CommandExecution/MysqlCreateIndexService.h"
#include "MysqlMigrator/CommandExecution/MysqlCreateTableService.h"
#include "MysqlMigrator/CommandExecution/MysqlDropColumnService.h"
#include "MysqlMigrator/CommandExecution/MysqlDropIndexService.h"
#include "MysqlMigrator/CommandExecution/MysqlDropTableService.h"
#include "MysqlMigrator/CommandExecution/MysqlRenameColumnService.h"
#include "MysqlMigrator/CommandExecution/MysqlRenameTableService.h"

#include "CommandExecution/CustomCommandService.h"

#include "MigrationTracker/MysqlMigrationTableService.h"

namespace MysqlMigrator {

using namespace CommandExecution;

QSharedPointer<CommandExecutionServiceRepository> commandServiceRepository()
{
    QSharedPointer<CommandExecutionServiceRepository> commandRepository(new CommandExecutionServiceRepository);
    commandRepository->add(BaseCommandServicePtr(new MysqlAddColumnService));
    commandRepository->add(BaseCommandServicePtr(new MysqlAlterColumnTypeService));
    commandRepository->add(BaseCommandServicePtr(new MysqlCreateIndexService));
    commandRepository->add(BaseCommandServicePtr(new MysqlCreateTableService));
    commandRepository->add(BaseCommandServicePtr(new MysqlDropColumnService));
    commandRepository->add(BaseCommandServicePtr(new MysqlDropIndexService));
    commandRepository->add(BaseCommandServicePtr(new MysqlDropTableService));
    commandRepository->add(BaseCommandServicePtr(new MysqlRenameColumnService));
    commandRepository->add(BaseCommandServicePtr(new MysqlRenameTableService));
    commandRepository->add(BaseCommandServicePtr(new CustomCommandService));

    return commandRepository;
}

} // namespace MysqlMigrator
