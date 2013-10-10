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
#ifndef MIGRATIONEXECUTION_MIGRATIONEXECUTIONCONTEXT_H
#define MIGRATIONEXECUTION_MIGRATIONEXECUTIONCONTEXT_H

#include "MigrationExecution/MigrationExecutionConfig.h"

#include "Helper/HelperAggregate.h"

#include <QMap>
#include <QSharedPointer>
#include <QSqlDatabase>

class QSqlDatabase;

namespace CommandExecution {
class CommandExecutionServiceRepository;
}
namespace Helper {
class HelperAggregate;
}
namespace Migrations {
class Migration;
}
namespace MigrationTracker {
class BaseMigrationTrackerService;
}

namespace MigrationExecution {

typedef QSharedPointer<CommandExecution::CommandExecutionServiceRepository> CommandServiceRepositoryPtr;
typedef QSharedPointer<Helper::HelperAggregate> HelperAggregatePtr;
typedef QSharedPointer<MigrationTracker::BaseMigrationTrackerService> MigrationTableServicePtr;

class QSQLMIGRATOR_DLL_EXPORT MigrationExecutionContext
{
public:
     typedef QMap<QString, const Migrations::Migration*> NameMigrationMap;

     MigrationExecutionContext(const NameMigrationMap &migrations, const MigrationExecutionConfig &migrationConfig);
     MigrationExecutionContext(const NameMigrationMap &migrations);

     MigrationTableServicePtr baseMigrationTableService() const;
     CommandServiceRepositoryPtr commandServiceRepository() const;
     const Helper::HelperAggregate &helperAggregate() const;
     QSqlDatabase database() const;
     const MigrationExecutionConfig &migrationConfig() const;
     const NameMigrationMap &migrationMap() const;

     void setBaseMigrationTableService(MigrationTableServicePtr baseMigrationTableService);
     void setCommandServiceRepository(CommandServiceRepositoryPtr commandServiceRepository);
     void setHelperAggregate(const Helper::HelperAggregate &helperAggregate);
     void setDatabase(QSqlDatabase database);

private:
     const NameMigrationMap m_migrations;
     MigrationExecutionConfig m_migrationConfig;
     CommandServiceRepositoryPtr m_commandServiceRepository;
     Helper::HelperAggregate m_helperAggregate;
     QSqlDatabase m_database;
     MigrationTableServicePtr m_migrationTableService;
};

} // namespace MigrationExecution

#endif // MIGRATIONEXECUTION_MIGRATIONEXECUTIONCONTEXT_H
