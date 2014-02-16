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
#ifndef MIGRATIONTRACKER_MIGRATIONTRACKERSERVICE_H
#define MIGRATIONTRACKER_MIGRATIONTRACKERSERVICE_H

#include "CommandExecution/CommandExecutionContext.h"

#include <QStringList>

namespace MigrationExecution{
class MigrationExecutionContext;
}

namespace MigrationTracker {

/*!
 * \brief Interface of a service that is used to track successfully executed migrations
 */
class QSQLMIGRATOR_DLL_EXPORT MigrationTrackerService
{
public:
    virtual ~MigrationTrackerService() {}

    //! \returns true if transactions can revert changes to table structures
    virtual bool canRevertStrucuturalChangesUsingTransactions() const = 0;

    //! \return a list of all tracked successful migration executions
    virtual QStringList migrationList(const CommandExecution::CommandExecutionContext &context) const = 0;

    //! \returns true if the specified migrations was tracked as successfully executed
    virtual bool wasMigrationExecuted(const QString &migrationName,
                                      const CommandExecution::CommandExecutionContext &context) const = 0;

    //! \return true, if it was able to sucessfully record that the migrations was executed
    virtual bool addMigration(const QString &migrationName,
                              const CommandExecution::CommandExecutionContext &context) const = 0;

    //! \return true, if it was able to sucessfully remove a migration from the tracked successfully executed migrations
    virtual bool removeMigration(const QString &migrationName,
                                 const CommandExecution::CommandExecutionContext &context) const = 0;

    //! \return true, if the tracking was in place or was sucessfully prepared
    virtual bool prepare(const MigrationExecution::MigrationExecutionContext &context) const = 0;
};

} // namespace MigrationTracker

#endif // MIGRATIONTRACKER_MIGRATIONTRACKERSERVICE_H
