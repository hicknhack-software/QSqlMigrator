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
#pragma once

#include "SqlSchemaCommand/Command.h"
#include "SqlDatabaseAdapter/Adapter.h"
#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

/// \brief collective state value for executors
class CommandExecutorContext {
public:
    using SqlAdapter = SqlDatabaseAdapter::Adapter;
    using Database = SqlAdapter::Database;
    struct Config {
        QString temporaryTablePrefix =
            "temp_"; ///< used by some databases who have to duplicate a table to apply schema changes
    };
    using Logging = QSharedPointer<LoggingTrace::Logging>;

    class Undo {
    public:
        using Command = QSharedPointer<const SqlSchemaCommand::Command>;
    public:
        /// \return true, when undo command will be used
        bool isUsed() const Q_DECL_NOEXCEPT;

        Command command() const;

        /// change if undo is relevant
        void setIsUsed(bool isUsed);

        /// store the undo command for the currently executed command
        void setCommand(Command command);

        /// clear the stored undo command
        void resetCommand();

    private:
        bool m_isUsed = false;
        Command m_command;
    };

public:
    CommandExecutorContext(SqlAdapter, Config, Logging);

    Database database() const;
    const SqlAdapter &sqlAdapter() const Q_DECL_NOEXCEPT;
    const Config &config() const Q_DECL_NOEXCEPT;
    const Logging &logging() const Q_DECL_NOEXCEPT;
    Undo &undo() Q_DECL_NOEXCEPT;

private:
    Database m_database;
    SqlAdapter m_sqlAdapter;
    Config m_config;
    Logging m_logging;
    Undo m_undo;
};

inline CommandExecutorContext::CommandExecutorContext(SqlAdapter sqlAdapter,
                                                      Config config,
                                                      Logging logging)
    : m_sqlAdapter(std::move(sqlAdapter))
    , m_config(std::move(config))
    , m_logging(std::move(logging))
{
}

inline CommandExecutorContext::Database
CommandExecutorContext::database() const
{
    return m_sqlAdapter.database();
}

inline const CommandExecutorContext::SqlAdapter &
CommandExecutorContext::sqlAdapter() const Q_DECL_NOEXCEPT
{
    return m_sqlAdapter;
}

inline const CommandExecutorContext::Config &
CommandExecutorContext::config() const Q_DECL_NOEXCEPT
{
    return m_config;
}

inline const CommandExecutorContext::Logging &
CommandExecutorContext::logging() const Q_DECL_NOEXCEPT
{
    return m_logging;
}

inline CommandExecutorContext::Undo &
CommandExecutorContext::undo() Q_DECL_NOEXCEPT
{
    return m_undo;
}

inline bool
CommandExecutorContext::Undo::isUsed() const Q_DECL_NOEXCEPT
{
    return m_isUsed;
}

inline CommandExecutorContext::Undo::Command
CommandExecutorContext::Undo::command() const
{
    return m_command;
}

inline void
CommandExecutorContext::Undo::setIsUsed(bool isUsed)
{
    m_isUsed = isUsed;
}

inline void
CommandExecutorContext::Undo::setCommand(Command command)
{
    m_command = command;
}

inline void
CommandExecutorContext::Undo::resetCommand()
{
    m_command.clear();
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
