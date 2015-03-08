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
#include "CommandExecutorRepository.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

void
CommandExecutorRepository::add(CommandExecutor commandExecutor)
{
    if (!commandExecutor)
        return; // empty executor

    const auto commandName = commandExecutor->commandType();
    const auto it =
        std::find_if(m_commandExecutors.begin(), m_commandExecutors.end(),
                     [&](const CommandExecutor &executor) { return executor->commandType() == commandName; });
    if (it == m_commandExecutors.end())
        m_commandExecutors << commandExecutor;
    else
        *it = commandExecutor;
}

CommandExecutorRepository::CommandExecutor
CommandExecutorRepository::findExecutor(const CommandName &commandName) const
{
    if (commandName.isEmpty())
        return {}; // empty name is not allowed

    auto it = std::find_if(m_commandExecutors.begin(), m_commandExecutors.end(),
                           [&](const CommandExecutor &executor) { return executor->commandType() == commandName; });
    if (it == m_commandExecutors.end())
        return {}; // not found

    return *it;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
