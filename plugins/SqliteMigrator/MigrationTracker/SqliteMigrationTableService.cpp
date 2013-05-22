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

const bool SqliteMigrationTableService::canRevertStrucuturalChangesUsingTransactions() const
{
    return true;
}

bool SqliteMigrationTableService::wasMigrationExecuted(const QString &migrationName
                                                               , const CommandExecutionContext &context) const
{
    this->initDb(context);
    return this->migrationList(context).contains(migrationName);
}

const QStringList SqliteMigrationTableService::migrationList(const CommandExecutionContext &context) const
{
    this->initDb(context);

    QSqlQuery query = context.database().exec("SELECT version FROM "
                                              + context.migrationConfig().migrationVersionTableName);

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
    this->initDb(context);

    QString sQuery = "INSERT INTO %1 VALUES ('%2')";
    sQuery = sQuery.arg(context.migrationConfig().migrationVersionTableName
                        , migrationName);
    QSqlQuery query = context.database().exec(sQuery);
    if (query.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << query.lastError().text();
        return false;
    }
    return true;
}

bool SqliteMigrationTableService::removeMigration(const QString &migrationName, const CommandExecutionContext &context
                                                  ) const
{
    this->initDb(context);

    QString sQuery = "DELETE FROM %1 WHERE version = '%2'";
    sQuery = sQuery.arg(context.migrationConfig().migrationVersionTableName
                        , migrationName);
    QSqlQuery query = context.database().exec(sQuery);
    if (query.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << query.lastError().text();
        return false;
    }
    return true;
}

void SqliteMigrationTableService::initDb(const CommandExecutionContext &context) const
{
    ::qDebug() << "init DB";
    QStringList tables = context.database().tables(QSql::AllTables);
    QString sTableName = context.migrationConfig().migrationVersionTableName;
    if (!tables.contains(sTableName)) {
        QString sQuery = "CREATE TABLE %1 (version VARCHAR(255) PRIMARY KEY)";
        QSqlQuery query = context.database().exec(sQuery.arg(sTableName));
        if (query.lastError().isValid())
            ::qDebug() << Q_FUNC_INFO << query.lastError().text();
    }
}

} // namespace MigrationTracker
