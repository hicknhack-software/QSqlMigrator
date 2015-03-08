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

#include "SqlDatabaseSchemaAdapter/CommandExecutorContext.h"
#include "SqlSchemaCommand/Command.h"

#include "config.h"

#include <QString>
#include <QSharedPointer>

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

///! \brief abstact base class for executors that runs a command against a specific SQL Database
class QSQLMIGRATOR_DLL_EXPORT CommandExecutor
{
public:
    using CommandType = SqlSchemaCommand::Command::CommandName;
    using Command = QSharedPointer<const QSqlMigrator::SqlSchemaCommand::Command>;
    using Context = CommandExecutorContext;
    using Sql = QString;

public:
    ///! \return the TypeName of the Command this Executor implements
    virtual const CommandType &commandType() const = 0;

    ///! \return true if the command was successfully applied to the database
    virtual bool execute(const Command &, Context &) const = 0;

    ///! \return true if the command can be applied against to the database, otherwise reports errors to logging
    virtual bool isValid(const Command &, const Context &) const = 0;

protected:
    ///! \return true if the SQL query was executed successfully
    static bool executeQuery(const Sql &, const Context &);
};

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
