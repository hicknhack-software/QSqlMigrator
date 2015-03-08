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
#pragma once

#include "LoggingTrace/Logging.h"
#include "SqlDatabaseAdapter/Adapter.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutorRepository.h"
#include "SqlMigration/MigrationTracker.h"

#include <QMap>
#include <QObject>
#include <QtTest>
#include <QString>

#include <QSqlDatabase>

#define TEST_CONNECTION_NAME "test"

#ifndef QTEST_GUILESS_MAIN
#define QTEST_GUILESS_MAIN QTEST_MAIN
#endif

class BasicTest : public QObject
{
    Q_OBJECT

protected:
    using Logging = QSharedPointer< QSqlMigrator::LoggingTrace::Logging >;
    using Adapter = QSqlMigrator::SqlDatabaseAdapter::Adapter;
    using CommandExecutors = QSqlMigrator::SqlDatabaseSchemaAdapter::CommandExecutorRepository;
    using MigrationTracker = QSharedPointer<QSqlMigrator::SqlMigration::MigrationTracker>;

protected:
    BasicTest(const QString &driverName, const QString &testDatabaseName, const QString &structureDatabaseName = "",
              const QString &hostName = "", const int hostPort = 0, const QString &userName = "", const QString &password = "");

    void initLibraryPath();

    virtual Adapter buildAdapter(QSqlDatabase) = 0;
    virtual CommandExecutors commandExecutors() = 0;
    virtual MigrationTracker migrationTracker();

    virtual void defineStructureDatabase();
    virtual void createStructureDatabase();
    virtual void cleanStructureDatabase();
    virtual void defineTestDatabase();
    void closeTestDatabase();

private:
    template< typename Command, typename... Args >
    bool execCommand(Args &&...);

private Q_SLOTS:
    //TODO add tests for "default value", "autoincrement" and other constraints
    //TODO check if versionTable is maintained correctly
    virtual void initTestCase();
    virtual void cleanupTestCase();
    virtual void init();
    virtual void cleanup();

    void testCreateTable();
    void testDropTable();
    void testDropTableRevert();
    void testMigrationDirections();
    void testTransaction();
    void testUndoCreateTable();
    void testUndoDropTable();
    void testAlterColumnType();
    void testDropColumn();
    void testRenameColumn();
    void testColumnType();
    void testLocalSchemeMigration();
    void testCreateIndex();

protected:
    const QString m_driverName;
    const QString m_testDatabaseName;
    const QString m_structureDatabaseName;

    const QString m_hostName;
    const int m_hostPort;
    const QString m_userName;
    const QString m_password;

    Logging m_logging;
    Adapter m_adapter;
};
