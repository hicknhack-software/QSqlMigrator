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

#include "config.h"

#include "SqlMemorySchema/SchemaCommandExecutorRepository.h"

#include "SqlMigration/MigrationRepository.h"

namespace QSqlMigrator {
namespace SqlMemorySchema {

/** \return a command repository for the standard migration commands */
SchemaCommandExecutorRepository createCommandRepository();

class QSQLMIGRATOR_DLL_EXPORT SchemaMigrator
{
public:
    using Migrations = SqlMigration::MigrationRepository;
    using CommandExecutors = SchemaCommandExecutorRepository;
    using Logging = QSharedPointer<LoggingTrace::Logging>;
    using Schema = SqlSchema::Schema;

    struct Setup {
        Migrations migrations;
        CommandExecutors commandExecutors;
    };
    struct Context {
        Schema schema;
        Logging logging;
    };

    using MigrationName = QString;
    using MigrationNames = QStringList;

public:
    SchemaMigrator(Setup);

    bool applyMigration(const MigrationName&, Context&) const;

    bool applyAll(const MigrationNames&, Context&) const;

private:
    Setup m_setup;
};

inline SchemaMigrator::SchemaMigrator(Setup setup)
    : m_setup(setup)
{}

} // namespace SqlMemorySchema
} // namespace QSqlMigrator
