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
#include "SqliteMigrator.h"

#include "CommandExecution/SqliteAddColumnService.h"
#include "CommandExecution/SqliteAlterColumnTypeService.h"
#include "CommandExecution/SqliteCreateIndexService.h"
#include "CommandExecution/SqliteCreateTableService.h"
#include "CommandExecution/SqliteDropColumnService.h"
#include "CommandExecution/SqliteDropIndexService.h"
#include "CommandExecution/SqliteDropTableService.h"
#include "CommandExecution/SqliteRenameColumnService.h"
#include "CommandExecution/SqliteRenameTableService.h"
#include "CommandExecution/CustomCommandService.h"

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

} // namespace SqliteMigrator
