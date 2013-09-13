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
#include "CommandExecutionContext.h"

namespace CommandExecution {

CommandExecutionContext::CommandExecutionContext(const QSqlDatabase database, const MigrationExecution::MigrationExecutionConfig &migrationConfig)
    : m_database(database)
    , m_migrationConfig(migrationConfig)
    , m_isUndoUsed(false)
{
}

const QSqlDatabase CommandExecutionContext::database() const
{
    return m_database;
}

bool CommandExecutionContext::isUndoUsed() const
{
    return m_isUndoUsed;
}

void CommandExecutionContext::setUndoCommand(Commands::CommandPtr command)
{
    if(!m_isUndoUsed) {
        return;
    }
    m_currentUndoCommand = command;
}

void CommandExecutionContext::resetUndoCommand()
{
    m_currentUndoCommand.clear();
}

Commands::CommandPtr CommandExecutionContext::currentUndoCommand() const
{
    return m_currentUndoCommand;
}

const MigrationExecution::MigrationExecutionConfig &CommandExecutionContext::migrationConfig() const
{
    return m_migrationConfig;
}

void CommandExecutionContext::setIsUndoUsed(bool isUndoUsed)
{
    m_isUndoUsed = isUndoUsed;
}

} // namespace CommandExecution
