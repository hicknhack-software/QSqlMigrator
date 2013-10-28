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
#include "LocalSchemeMigrationExecutionContext.h"
#include "Migrations/Migration.h"
#include "CommandExecution/LocalSchemeCommandExecutionServiceRepository.h"

namespace MigrationExecution {

LocalSchemeMigrationExecutionContext::LocalSchemeMigrationExecutionContext(const LocalSchemeMigrationExecutionContext::NameMigrationMap &migrations)
    : m_migrations(migrations)
{
}

LocalSchemePtr &LocalSchemeMigrationExecutionContext::localScheme()
{
    return m_localScheme;
}

const LocalSchemeMigrationExecutionContext::NameMigrationMap &LocalSchemeMigrationExecutionContext::migrationMap() const
{
    return m_migrations;
}

void LocalSchemeMigrationExecutionContext::setLocalScheme(const LocalSchemePtr localScheme)
{
    m_localScheme = localScheme;
}

CommandExecution::LocalSchemeCommandServiceRepositoryPtr LocalSchemeMigrationExecutionContext::localSchemeCommandServiceRepository() const
{
    return m_commandServiceRepository;
}

void LocalSchemeMigrationExecutionContext::setLocalSchemeCommandServiceRepository(LocalSchemeCommandServiceRepositoryPtr commandServiceRepository)
{
    m_commandServiceRepository = commandServiceRepository;
}

} // namespace MigrationExecution
