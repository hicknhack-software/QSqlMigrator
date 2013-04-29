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
#ifndef MIGRATIONEXECUTION_MIGRATIONEXECUTIONSERVICE_H
#define MIGRATIONEXECUTION_MIGRATIONEXECUTIONSERVICE_H

#include "MigrationExecution/MigrationExecutionContext.h"
#include "CommandExecution/CommandExecutionService.h"

namespace MigrationExecution {

class QSQLMIGRATOR_DLL_EXPORT MigrationExecutionService
{
public:
    enum Direction {
        Up,
        Down
    };

    MigrationExecutionService();

    bool execute(const QString &migrationName
                 , const MigrationExecutionContext &migrationContext
                 , Direction direction = Up) const;

    bool executeBatch(const QStringList &migrationList
                      , const MigrationExecutionContext &context
                      , Direction direction = Up) const;

private:
    bool isMigrationRemembered(const QString &migrationName
                               , const MigrationExecutionContext &context
                               , Direction direction) const;

    bool rememberMigration(const QString &migrationName
                           , const MigrationExecutionContext &context
                           , Direction direction) const;

private:
    CommandExecution::CommandExecutionService m_execution;
};

} //namespace MigrationExecution

#endif // MIGRATIONEXECUTION_MIGRATIONEXECUTIONSERVICE_H
