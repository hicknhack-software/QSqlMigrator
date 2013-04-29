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
#include "MysqlMigrationTableService.h"
#include "Helper/MysqlQuoteService.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

namespace MigrationTracker {

MysqlMigrationTableService::MysqlMigrationTableService()
{
}

const bool MysqlMigrationTableService::canRevertStrucuturalChangesUsingTransactions() const
{
    return false;
}

bool MysqlMigrationTableService::wasMigrationExecuted(const QString &migrationName
                                                      , const CommandExecution::CommandExecutionContext &context) const
{
    this->initDb(context);
    return this->migrationList(context).contains(migrationName);
}

const QStringList MysqlMigrationTableService::migrationList(const CommandExecution::CommandExecutionContext &context) const
{
    this->initDb(context);

    QSqlQuery query = context.database().exec("SELECT version FROM "
                                              + Helper::MysqlQuoteService::quoteTableName(
                                                  context.migrationConfig().migrationVersionTableName));

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

bool MysqlMigrationTableService::addMigration(const QString &migrationName
                                              , const CommandExecution::CommandExecutionContext &context) const
{
    this->initDb(context);

    QString sQuery = "INSERT INTO %1 (version) VALUE (%2)";
    sQuery = sQuery.arg(Helper::MysqlQuoteService::quoteTableName(
                            context.migrationConfig().migrationVersionTableName)
                        , Helper::MysqlQuoteService::quoteString(migrationName));
    ::qDebug() << "adding entry to version table. complete query looks like:";
    ::qDebug() << sQuery;
    QSqlQuery query = context.database().exec(sQuery);
    if (query.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << query.lastError().text();
        return false;
    }
    return true;
}

bool MysqlMigrationTableService::removeMigration(const QString &migrationName
                                                 , const CommandExecution::CommandExecutionContext &context) const
{
    this->initDb(context);

    QString sQuery = "DELETE FROM %1 WHERE version = %2";
    sQuery = sQuery.arg(Helper::MysqlQuoteService::quoteTableName(
                            context.migrationConfig().migrationVersionTableName)
                        , Helper::MysqlQuoteService::quoteString(migrationName));
    QSqlQuery query = context.database().exec(sQuery);
    if (query.lastError().isValid()) {
        ::qDebug() << Q_FUNC_INFO << query.lastError().text();
        return false;
    }
    return true;
}

void MysqlMigrationTableService::initDb(const CommandExecution::CommandExecutionContext &context) const
{
    ::qDebug() << "init DB";
    QStringList tables = context.database().tables(QSql::AllTables);
    QString sTableName = context.migrationConfig().migrationVersionTableName;
    if (!tables.contains(sTableName)) {
        QString sQuery = "CREATE TABLE %1 (version varchar(255) NOT NULL, PRIMARY KEY (version))";
        ::qDebug() << "creating migrationVersion table! query looks like:";
        ::qDebug() << sQuery.arg(Helper::MysqlQuoteService::quoteTableName(sTableName));
        QSqlQuery query = context.database().exec(sQuery.arg(Helper::MysqlQuoteService::quoteTableName(sTableName)));
        if (query.lastError().isValid())
            ::qDebug() << Q_FUNC_INFO << query.lastError().text();
    }
}

} // namespace MigrationTracker
