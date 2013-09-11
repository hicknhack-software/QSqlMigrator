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
#include "QSqlMigratorConfig.h"

namespace QSqlMigrator {

const QString &QSqlMigratorConfig::databaseType() const
{
    return m_databaseType;
}

const QString &QSqlMigratorConfig::databaseHostName() const
{
    return m_databseHostName;
}

const QString &QSqlMigratorConfig::databasePort() const
{
    return m_databasePort;
}

const QString &QSqlMigratorConfig::databaseUserName() const
{
    return m_databaseUserName;
}

const QString &QSqlMigratorConfig::databasePassword() const
{
    return m_databasePassword;
}

const QString &QSqlMigratorConfig::databasename() const
{
    return m_databaseName;
}

const QString &QSqlMigratorConfig::migrationTableName() const
{
    return m_migrationConfig.migrationVersionTableName;
}

const QString &QSqlMigratorConfig::migrationTablePrefix() const
{
    return m_migrationConfig.migrationTablePrefix;
}

const QString &QSqlMigratorConfig::temporaryTablePrefix() const
{
    return m_migrationConfig.temporaryTablePrefix;
}

bool QSqlMigratorConfig::migrationCreateTable() const
{
    return m_migrationConfig.createDatabase;
}

void QSqlMigratorConfig::setDatabaseType(const QString &databaseType)
{
    m_databaseType = databaseType;
}

void QSqlMigratorConfig::setDatabaseHostName(const QString &host)
{
    m_databseHostName = host;
}

void QSqlMigratorConfig::setDatabasePort(const QString &port)
{
    m_databasePort = port;
}

void QSqlMigratorConfig::setDatabaseUserName(const QString &user)
{
    m_databaseUserName = user;
}

void QSqlMigratorConfig::setDatabasePassword(const QString &password)
{
    m_databasePassword = password;
}

void QSqlMigratorConfig::setDatabasename(const QString &databaseName)
{
    m_databaseName = databaseName;
}

void QSqlMigratorConfig::setMigrationTableName(const QString &migrationTableName)
{
    m_migrationConfig.migrationVersionTableName = migrationTableName;
}

void QSqlMigratorConfig::setMigrationTablePrefix(const QString &tablePrefix)
{
    m_migrationConfig.migrationTablePrefix = tablePrefix;
}

void QSqlMigratorConfig::setTemporaryTablePrefix(const QString &temporaryTablePrefix)
{
    m_migrationConfig.temporaryTablePrefix = temporaryTablePrefix;
}

void QSqlMigratorConfig::setMigrationCreateTable(const bool createDatabase)
{
    m_migrationConfig.createDatabase = createDatabase;
}

} // namespace QSqlMigrator
