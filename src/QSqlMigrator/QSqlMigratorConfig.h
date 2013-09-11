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
#ifndef QSQLMIGRATOR_QSQLMIGRATORCONFIG_H
#define QSQLMIGRATOR_QSQLMIGRATORCONFIG_H

#include "MigrationExecution/MigrationExecutionConfig.h"

#include <QString>

namespace QSqlMigrator {

/*!
 * \brief The QSqlMigratorConfig class is used to represent all configurations of interest
 * for the QSqlMigrator
 */
class QSQLMIGRATOR_DLL_EXPORT QSqlMigratorConfig
{
public:
     const QString &databaseType() const;
     const QString &databaseHostName() const;
     const QString &databasePort() const;
     const QString &databaseUserName() const;
     const QString &databasePassword() const;
     const QString &databasename() const;

     const QString &migrationTableName() const;
     const QString &migrationTablePrefix() const;
     const QString &temporaryTablePrefix() const;
     bool migrationCreateTable() const;

     void setDatabaseType(const QString &databaseType);
     void setDatabaseHostName(const QString &host);
     void setDatabasePort(const QString &port);
     void setDatabaseUserName(const QString &user);
     void setDatabasePassword(const QString &password);
     void setDatabasename(const QString &databaseName);

     void setMigrationTableName(const QString &migrationTableName);
     void setMigrationTablePrefix(const QString &tablePrefix);
     void setTemporaryTablePrefix(const QString &temporaryTablePrefix);
     void setMigrationCreateTable(const bool createDatabase);

private:
     QString m_databaseType;
     QString m_databseHostName;
     QString m_databasePort;
     QString m_databaseUserName;
     QString m_databasePassword;
     QString m_databaseName;

     MigrationExecution::MigrationExecutionConfig m_migrationConfig;
};

} // namespace QSqlMigrator

#endif // QSQLMIGRATOR_QSQLMIGRATORCONFIG_H
