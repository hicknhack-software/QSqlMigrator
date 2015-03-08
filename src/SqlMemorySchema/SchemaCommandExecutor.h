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

#include "LoggingTrace/Logging.h"

#include "SqlSchema/Schema.h"

#include "SqlSchemaCommand/Command.h"

#include "config.h"

#include <QSharedPointer>

namespace QSqlMigrator {
namespace SqlMemorySchema {

///! \brief abstact base class for executors that change the schema in memory
class QSQLMIGRATOR_DLL_EXPORT SchemaCommandExecutor
{
public:
    using CommandType = SqlSchemaCommand::Command::CommandName;
    using Command = QSharedPointer<const SqlSchemaCommand::Command>;
    using Logging = QSharedPointer<LoggingTrace::Logging>;
    using Schema = SqlSchema::Schema;

    struct Context {
        Schema schema;
        Logging logging;
    };

public:
    ///! \return the TypeName of the Command this Executor implements
    virtual const CommandType &commandType() const = 0;

    ///! \return true if the command was successfully applied to the schema
    virtual bool execute(const Command &, Context &) const = 0;

    ///! \return true if the command can be applied against the schema
    virtual bool isValid(const Command &, const Context &) const = 0;
};

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
