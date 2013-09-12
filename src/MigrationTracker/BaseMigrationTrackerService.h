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
#ifndef MIGRATIONTRACKER_BASEMIGRATIONTRACKERSERVICE_H
#define MIGRATIONTRACKER_BASEMIGRATIONTRACKERSERVICE_H

#include "CommandExecution/CommandExecutionContext.h"

#include <QStringList>

namespace MigrationTracker {

class QSQLMIGRATOR_DLL_EXPORT BaseMigrationTrackerService
{
public:
    virtual ~BaseMigrationTrackerService() {}

    virtual bool canRevertStrucuturalChangesUsingTransactions() const = 0;

    virtual QStringList migrationList(const CommandExecution::CommandExecutionContext &context) const = 0;

    virtual bool wasMigrationExecuted(const QString &migrationName
                                      , const CommandExecution::CommandExecutionContext &context) const = 0;

    virtual bool addMigration(const QString &migrationName
                              , const CommandExecution::CommandExecutionContext &context) const = 0;

    virtual bool removeMigration(const QString &migrationName
                                 , const CommandExecution::CommandExecutionContext &context) const = 0;

    virtual bool ensureVersionTable(const CommandExecution::CommandExecutionContext &context) const = 0;
};

} // namespace MigrationTracker

#endif // MIGRATIONTRACKER_BASEMIGRATIONTRACKERSERVICE_H
