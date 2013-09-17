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
#include "SqliteMigrator/MigrationTracker/SqliteMigrationTableService.h"

#include "MigrationExecution/MigrationExecutionContext.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>


using namespace CommandExecution;

namespace MigrationTracker {

SqliteMigrationTableService::SqliteMigrationTableService()
{
}

bool SqliteMigrationTableService::canRevertStrucuturalChangesUsingTransactions() const
{
    return true;
}

bool SqliteMigrationTableService::wasMigrationExecuted(const QString &migrationName
                                                               , const CommandExecutionContext &context) const
{
    return this->migrationList(context).contains(migrationName);
}

QStringList SqliteMigrationTableService::migrationList(const CommandExecutionContext &context) const
{
    QString versionTableName = context.migrationConfig().migrationVersionTableName;
    QSqlQuery query = context.database().exec("SELECT version FROM "
                                              + versionTableName);

    QStringList lMigrationList;
    if (query.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << query.lastError().text();
    } else {
        while (query.next()) {
            lMigrationList << query.value(0).toString();
        }
    }
    return lMigrationList;
}

bool SqliteMigrationTableService::addMigration(const QString &migrationName, const CommandExecutionContext &context
                                               ) const
{
    QString versionTableName = context.migrationConfig().migrationVersionTableName;
    QString query = "INSERT INTO %1 VALUES ('%2')";
    query = query.arg(versionTableName, migrationName);
    QSqlQuery sqlQuery = context.database().exec(query);
    if (sqlQuery.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << sqlQuery.lastError().text();
        return false;
    }
    return true;
}

bool SqliteMigrationTableService::removeMigration(const QString &migrationName, const CommandExecutionContext &context
                                                  ) const
{
    QString versionTableName = context.migrationConfig().migrationVersionTableName;
    QString query = "DELETE FROM %1 WHERE version = '%2'";
    query = query.arg(versionTableName, migrationName);
    QSqlQuery sqlQuery = context.database().exec(query);
    if (sqlQuery.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << sqlQuery.lastError().text();
        return false;
    }
    return true;
}

bool SqliteMigrationTableService::ensureVersionTable(const MigrationExecution::MigrationExecutionContext &context) const
{
    QStringList tables = context.database().tables(QSql::AllTables);
    QString versionTableName = context.migrationConfig().migrationVersionTableName;
    if(!tables.contains(versionTableName)) {
        QString query = "CREATE TABLE %1 (version VARCHAR(255) PRIMARY KEY)";
        ::qDebug() << "creating: " << versionTableName;
        QSqlQuery sqlQuery = context.database().exec(query.arg(versionTableName));
        if (sqlQuery.lastError().isValid())
        {
            ::qDebug() << Q_FUNC_INFO << sqlQuery.lastError().text();
            return false;
        }
    }
    return true;
}

} // namespace MigrationTracker
