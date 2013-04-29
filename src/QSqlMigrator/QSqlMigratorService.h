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
#ifndef QSQLMIGRATOR_QSQLMIGRATORSERVICE_H
#define QSQLMIGRATOR_QSQLMIGRATORSERVICE_H

#include "config.h"

#include <QStringList>

namespace MigrationExecution {
class MigrationExecutionContext;
} // namespace MigrationExecution

namespace QSqlMigrator {

/*!
 * \brief The QSqlMigratorService class is used to control all relevant features of the QSqlMigrator.
 * The User should use this service within it's own applications.
 */
class QSQLMIGRATOR_DLL_EXPORT QSqlMigratorService
{
public:
    QStringList appliedMigrations(const MigrationExecution::MigrationExecutionContext &context) const;
    QStringList definedMigrations(const MigrationExecution::MigrationExecutionContext &context) const;
    QString lastAppliedMigration(const MigrationExecution::MigrationExecutionContext &context) const;
    QStringList missingMigrations(const MigrationExecution::MigrationExecutionContext &context) const;
    QStringList unappliedMigrations(const MigrationExecution::MigrationExecutionContext &context) const;

    bool applyAll(const MigrationExecution::MigrationExecutionContext &context) const;
    bool applyMigration(const QString &migrationName, const MigrationExecution::MigrationExecutionContext &context) const;
    bool migrateTo(const QString &migrationName, const MigrationExecution::MigrationExecutionContext &context) const;
    bool revertMigration(const QString &migrationName, const MigrationExecution::MigrationExecutionContext &context) const;
};

} // namespace QSqlMigrator

#endif // QSQLMIGRATOR_QSQLMIGRATORSERVICE_H
