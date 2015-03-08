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
#include "SqliteMigrator/SqliteMigrator.h"

#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardAddColumn.h"
#include "SqliteMigrator/CommandExecutor/SqliteAlterColumnType.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardCreateIndex.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardCreateTable.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardCustom.h"
#include "SqliteMigrator/CommandExecutor/SqliteDropColumn.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardDropIndex.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardDropTable.h"
#include "SqliteMigrator/CommandExecutor/SqliteRenameColumn.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutor/StandardRenameTable.h"

#include "SqlDatabaseAdapter/StandardQuotation.h"
#include "SqlDatabaseAdapter/StandardColumnMapper.h"
#include "SqliteMigrator/Helper/SqliteSchemaReflection.h"
#include "SqlDatabaseAdapter/StandardValueTypeMapper.h"

#include "Helper/SqliteSchemaReflection.h"

#include "SqlDatabaseSchemaAdapter/CommandExecutorRepository.h"

#include "SqlMigration/DatabaseMigrationTracker.h"

#include <QSqlDatabase>

#include <QDebug>

namespace SqliteMigrator {

Adapter
createAdapter(QSqlDatabase database)
{
    using namespace QSqlMigrator::SqlDatabaseAdapter;

    const auto quotation = QSharedPointer<StandardQuotation>::create();
    const auto valueTypeMapper = QSharedPointer<StandardValueTypeMapper>::create();
    const auto columnMapper = QSharedPointer<StandardColumnMapper>::create(valueTypeMapper);
    const auto schemaReflection = QSharedPointer<SqliteSchemaReflection>::create();

    return Adapter{database, quotation, valueTypeMapper, columnMapper, schemaReflection};
}

CommandRepository
createCommandRepository()
{
    using namespace QSqlMigrator::SqlDatabaseSchemaAdapter;
    using namespace SqliteMigrator::SqlDatabaseSchemaAdapter;

    CommandRepository commandRepository;
    commandRepository.add(QSharedPointer<StandardAddColumn>::create());
    commandRepository.add(QSharedPointer<SqliteAlterColumnType>::create());
    commandRepository.add(QSharedPointer<StandardCreateIndex>::create());
    commandRepository.add(QSharedPointer<StandardCreateTable>::create());
    commandRepository.add(QSharedPointer<StandardCustom>::create());
    commandRepository.add(QSharedPointer<SqliteDropColumn>::create());
    commandRepository.add(QSharedPointer<StandardDropIndex>::create());
    commandRepository.add(QSharedPointer<StandardDropTable>::create());
    commandRepository.add(QSharedPointer<SqliteRenameColumn>::create());
    commandRepository.add(QSharedPointer<StandardRenameTable>::create());
    return commandRepository;
}

} // namespace SqliteMigrator
