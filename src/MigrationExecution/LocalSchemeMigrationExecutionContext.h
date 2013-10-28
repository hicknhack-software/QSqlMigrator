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
#ifndef MIGRATIONEXECUTION_LOCALSCHEMEMIGRATIONEXECUTIONCONTEXT_H
#define MIGRATIONEXECUTION_LOCALSCHEMEMIGRATIONEXECUTIONCONTEXT_H

#include "config.h"

#include "Structure/LocalScheme.h"

#include <QMap>
#include <QSharedPointer>

namespace CommandExecution {
class LocalSchemeCommandExecutionServiceRepository;
}
namespace Migrations {
class Migration;
}

namespace MigrationExecution {

typedef QSharedPointer<CommandExecution::LocalSchemeCommandExecutionServiceRepository> LocalSchemeCommandServiceRepositoryPtr;

class QSQLMIGRATOR_DLL_EXPORT LocalSchemeMigrationExecutionContext
{
public:
     typedef QMap<QString, const Migrations::Migration*> NameMigrationMap;

     LocalSchemeMigrationExecutionContext(const NameMigrationMap &migrations);

     const NameMigrationMap &migrationMap() const;
     LocalSchemePtr &localScheme();
     void setLocalScheme(const LocalSchemePtr localScheme);
     LocalSchemeCommandServiceRepositoryPtr localSchemeCommandServiceRepository() const;
     void setLocalSchemeCommandServiceRepository(LocalSchemeCommandServiceRepositoryPtr commandServiceRepository);

private:
     const NameMigrationMap m_migrations;
     LocalSchemePtr m_localScheme;
     LocalSchemeCommandServiceRepositoryPtr m_commandServiceRepository;
};

} // namespace MigrationExecution

#endif // MIGRATIONEXECUTION_LOCALSCHEMEMIGRATIONEXECUTIONCONTEXT_H
