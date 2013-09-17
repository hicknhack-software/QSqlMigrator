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
#include "MysqlMigrator/MigrationTracker/MysqlMigrationTableService.h"

#include "MysqlMigrator/Helper/MysqlQuoteService.h"
#include "MigrationExecution/MigrationExecutionContext.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

namespace MigrationTracker {

MysqlMigrationTableService::MysqlMigrationTableService()
{
}

bool MysqlMigrationTableService::canRevertStrucuturalChangesUsingTransactions() const
{
    return false;
}

bool MysqlMigrationTableService::wasMigrationExecuted(const QString &migrationName
                                                      , const CommandExecution::CommandExecutionContext &context) const
{
    return this->migrationList(context).contains(migrationName);
}

QStringList MysqlMigrationTableService::migrationList(const CommandExecution::CommandExecutionContext &context) const
{


    QString versionTableName = context.migrationConfig().migrationVersionTableName;
    QSqlQuery sqlQuery = context.database().exec("SELECT version FROM "
                                                 + Helper::MysqlQuoteService::quoteTableName(versionTableName));

    QStringList lMigrationList;
    if (sqlQuery.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << sqlQuery.lastError().text();
    } else {
        while (sqlQuery.next()) {
            lMigrationList << sqlQuery.value(0).toString();
        }
    }
    return lMigrationList;
}

bool MysqlMigrationTableService::addMigration(const QString &migrationName
                                              , const CommandExecution::CommandExecutionContext &context) const
{
    QString versionTableName = context.migrationConfig().migrationVersionTableName;
    QString query = "INSERT INTO %1 (version) VALUE (%2)";
    query = query.arg(Helper::MysqlQuoteService::quoteTableName(versionTableName)
                      , Helper::MysqlQuoteService::quoteString(migrationName));
    ::qDebug() << "adding entry to version table. complete query looks like:";
    ::qDebug() << query;
    QSqlQuery sqlQuery = context.database().exec(query);
    if (sqlQuery.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << sqlQuery.lastError().text();
        return false;
    }
    return true;
}

bool MysqlMigrationTableService::removeMigration(const QString &migrationName
                                                 , const CommandExecution::CommandExecutionContext &context) const
{
    QString versionTableName = context.migrationConfig().migrationVersionTableName;
    QString query = "DELETE FROM %1 WHERE version = %2";
    query = query.arg(Helper::MysqlQuoteService::quoteTableName(versionTableName)
                      , Helper::MysqlQuoteService::quoteString(migrationName));
    QSqlQuery sqlQuery = context.database().exec(query);
    if (sqlQuery.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << sqlQuery.lastError().text();
        return false;
    }
    return true;
}

bool MysqlMigrationTableService::ensureVersionTable(const MigrationExecution::MigrationExecutionContext &context) const
{
    QStringList tables = context.database().tables(QSql::AllTables);
    QString versionTableName = context.migrationConfig().migrationVersionTableName;
    if(!tables.contains(versionTableName))
    {
        QString query = "CREATE TABLE %1 (version varchar(255) NOT NULL, PRIMARY KEY (version))";
        ::qDebug() << "creating migrationVersion table! query looks like:";
        ::qDebug() << query.arg(Helper::MysqlQuoteService::quoteTableName(versionTableName));
        QSqlQuery sqlQuery = context.database().exec(query.arg(Helper::MysqlQuoteService::quoteTableName(versionTableName)));
        if (sqlQuery.lastError().isValid())
        {
            ::qDebug() << Q_FUNC_INFO << sqlQuery.lastError().text();
            return false;
        }
    }
    return true;
}

} // namespace MigrationTracker
