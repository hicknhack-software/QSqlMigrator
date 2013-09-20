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
#include "MigrationExecution/MigrationExecutionContext.h"
#include "Migrations/MigrationRepository.h"
#include "QSqlMigrator/QSqlMigratorService.h"
#include "SqliteMigrator/SqliteMigrator.h"
#include "SqliteMigrator/MigrationTracker/SqliteMigrationTableService.h"
#include "SqliteMigrator/DatabaseLock.h"

#include "ApiConfig.h"

#include <QString>
#include <QSqlDatabase>
#include <QtTest>

using namespace Migrations;
using namespace MigrationExecution;


class ApiTest : public QObject
{
    Q_OBJECT

public:
    ApiTest();

private Q_SLOTS:
    void initTestCase();
    void init();

    void testDatabaseWithLockFile();

    void testRegistrationMacro();
    void testDefinedMigrations();
    void testAppliedMigrations();
    void testLastAppliedMigration();
    void testApplyMigration();
    void testApplyAll();
    void testMigrateTo();
    void testMissingMigrations();
    void testRevertMigration();
    void testUnappliedMigrations();

    void cleanup();
    void cleanupTestCase();

private:
    MigrationExecutionContext m_context;
};

ApiTest::ApiTest() : m_context (MigrationRepository::migrations(), MigrationExecutionConfig())
{
}

void ApiTest::initTestCase()
{
    ::qDebug() << "running test for Api";
}

void ApiTest::cleanupTestCase()
{
    if (m_context.database().isOpen()) {
        m_context.database().close();
    }
    if (!QString(API_DATABASE_FILENAME).isEmpty()) {
        if (QFile::exists(API_DATABASE_FILENAME)) {
            QFile::remove(API_DATABASE_FILENAME);
        }
    }
}

void ApiTest::init()
{
    QSqlDatabase database;
    if(!QSqlDatabase::contains(/*default-connection*/)) {
        database = QSqlDatabase::addDatabase(API_DRIVERNAME);
        database.setDatabaseName(API_DATABASE);
    }
    else
        database = m_context.database();

    bool success = SqliteMigrator::buildContext(m_context, database);
    QVERIFY2(success, "context should be builded correctly!");
}

void ApiTest::cleanup()
{
    if (m_context.database().isOpen()) {
        m_context.database().close();
    }
    if (!QString(API_DATABASE_FILENAME).isEmpty()) {
        QFile::remove(API_DATABASE_FILENAME);
    }
}

void ApiTest::testDatabaseWithLockFile()
{
    QString dummyLockFileName = SqliteMigrator::DatabaseLock::buildLockFileName(m_context);
    QFile dummyLockFile(dummyLockFileName);
    {
        dummyLockFile.open(QIODevice::WriteOnly);
        dummyLockFile.write("test dummy lock");
        dummyLockFile.close();

        SqliteMigrator::DatabaseLock dummyLockReleaser(m_context);
    }
    QVERIFY2(!QFile::exists(dummyLockFileName), "dummy lock file shold NO more exist!");

    {
        SqliteMigrator::DatabaseLock lockA(m_context);
        QVERIFY2(lockA, "lockA shold be successful got the lock!");

        SqliteMigrator::DatabaseLock lockB(m_context);
        QVERIFY2(!lockB, "lockB should not got the lock, lockA should still have the lock!");
    }

    SqliteMigrator::DatabaseLock lockC(m_context);
    QVERIFY2(lockC, "lockC shold be successful got the lock!");
}


void ApiTest::testRegistrationMacro()
{
    QMap<QString, const Migration*> migrationList;
    migrationList = MigrationRepository::migrations();
    ::qDebug() << migrationList.keys();
    QVERIFY2(migrationList.contains("M20131501_191807_CreateUsers")
             , "migrationList should contain CreateUsers migration");
    QVERIFY2(migrationList.contains("M20132201_180943_CreateCars")
             , "migrationList should contain CreateCars migration");
    QVERIFY2(migrationList.contains("M20133001_164323_AddUsers")
             , "migrationList should contain AddUsers migration");
    QVERIFY2(migrationList.contains("M20132201_175827_CreateAddresses")
             , "migrationList should contain CreateAddresses migration");
}

void ApiTest::testDefinedMigrations()
{
    QSqlMigrator::QSqlMigratorService manager;
    QStringList migrations = manager.definedMigrations(m_context);
    QVERIFY2(migrations.contains("M20131501_191807_CreateUsers")
             , "migrationList should contain CreateUsers migration");
    //test if the list was sorted
    QVERIFY2(migrations.at(0) == "M20131501_191807_CreateUsers"
             , "createUsers must be the first due to its timestamp!");
    QVERIFY2(migrations.at(1) == "M20132201_175827_CreateAddresses"
             , "createAddresses must be the second due to its timestamp!");
    QVERIFY2(migrations.at(2) == "M20132201_180943_CreateCars"
             , "createCars must be the third due to its timestamp!");
    QVERIFY2(migrations.at(3) == "M20133001_164323_AddUsers"
             , "addUsers must be the fourth due to its timestamp!");
}

void ApiTest::testAppliedMigrations()
{
    QSqlMigrator::QSqlMigratorService manager;
    QStringList definedMigrations = manager.definedMigrations(m_context);
    QStringList appliedMigrations = manager.appliedMigrations(m_context);
    QVERIFY2(appliedMigrations.size() == 0, "no migration could be applied yet!");

    MigrationTracker::SqliteMigrationTableService tableService;
    CommandExecution::CommandExecutionContext serviceContext(m_context.database(), m_context.migrationConfig());
    tableService.addMigration(definedMigrations.at(0), serviceContext);
    appliedMigrations = manager.appliedMigrations(m_context);
    QVERIFY2(appliedMigrations.size() == 1, "one migration should be shown as applied");
    QVERIFY2(appliedMigrations.contains(definedMigrations.at(0))
             , "appliedMigrations should contain first of the defined migrations");
}

void ApiTest::testLastAppliedMigration()
{
    QSqlMigrator::QSqlMigratorService manager;
    QStringList definedMigrations = manager.definedMigrations(m_context);
    QString lastAppliedMigration = manager.lastAppliedMigration(m_context);
    QVERIFY2(lastAppliedMigration.isEmpty(), "no Migration could be applied yet!");

    MigrationTracker::SqliteMigrationTableService tableService;
    CommandExecution::CommandExecutionContext serviceContext(m_context.database(), m_context.migrationConfig());
    tableService.addMigration(definedMigrations.at(1), serviceContext);
    lastAppliedMigration = manager.lastAppliedMigration(m_context);
    QVERIFY2(lastAppliedMigration == definedMigrations.at(1), "second defined migration was applied last!");

    //at this point, 2 migrations are within the migrationTable, but hey are not sorted! nevertheless,
    //the last applied migration has to be the second of the defined migrations
    tableService.addMigration(definedMigrations.at(0), serviceContext);
    lastAppliedMigration = manager.lastAppliedMigration(m_context);
    QVERIFY2(lastAppliedMigration == definedMigrations.at(1), "second defined migration was applied last!");
}

void ApiTest::testApplyMigration()
{
    QSqlMigrator::QSqlMigratorService manager;
    bool success = manager.applyMigration("M20132201_175827_CreateAddresses", m_context);
    QVERIFY2(success, "applyMigration should return true");
    QStringList tables = m_context.database().tables(QSql::Tables);
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(tables.size() == 2, "only one table (+ migrationTable) should be created");
}

void ApiTest::testApplyAll()
{
    QSqlMigrator::QSqlMigratorService manager;
    bool success = manager.applyAll(m_context);
    QVERIFY2(success, "applyAll should return true");
    QStringList tables = m_context.database().tables();
    QVERIFY2(tables.contains("users"), "table 'users' should be created during migration");
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(tables.contains("cars"), "table 'cars' should be created during migration");
}

void ApiTest::testMigrateTo()
{
    QSqlMigrator::QSqlMigratorService manager;
    bool success = manager.migrateTo("M20131501_191807_CreateUsers", m_context);
    QVERIFY2(success, "migrateTo should return true");
    QStringList tables = m_context.database().tables(QSql::Tables);
    QVERIFY2(tables.size() == 2, "one table (+ migrationTable) should be created yet");
    QVERIFY2(tables.contains("users"), "table 'users' should be created during migration");
    success = manager.migrateTo("M20132201_180943_CreateCars", m_context);
    QVERIFY2(success, "migrateTo should return true");
    tables = m_context.database().tables(QSql::Tables);
    QVERIFY2(tables.size() == 4
             , "two tables should be added during second migrateTo(), makes three (+ migrationTable) tables overall");
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(tables.contains("cars"), "table 'cars' should be created during migration");
}

void ApiTest::testMissingMigrations()
{
    QSqlMigrator::QSqlMigratorService manager;
    MigrationTracker::SqliteMigrationTableService tableService;
    CommandExecution::CommandExecutionContext serviceContext(m_context.database(), m_context.migrationConfig());
    tableService.addMigration("M20132301_103512_MissingMigration", serviceContext);
    QStringList definedMigrations = manager.definedMigrations(m_context);
    QStringList appliedMigrations = manager.appliedMigrations(m_context);
    QVERIFY2(appliedMigrations.contains("M20132301_103512_MissingMigration")
             , "M20132301_103512_MissingMigration should be part of the appliedMigrationsList");
    QVERIFY2(!definedMigrations.contains("M20132301_103512_MissingMigration")
             , "M20132301_103512_MissingMigration should NOT be part of the definedMigrationsList");
    QStringList missingMigrations = manager.missingMigrations(m_context);
    QVERIFY2(missingMigrations.size() == 1, "one missing migration");
    QVERIFY2(missingMigrations.contains("M20132301_103512_MissingMigration")
             , "M20132301_103512_MissingMigration should be part of the missingMigrationsList");
}

void ApiTest::testRevertMigration()
{
    QSqlMigrator::QSqlMigratorService manager;
    bool success = manager.applyAll(m_context);
    QVERIFY2(success, "applyAll should return true");
    success = manager.revertMigration("M20132201_180943_CreateCars", m_context);
    QVERIFY2(success, "revertMigration should return true");
    QStringList tables = m_context.database().tables(QSql::Tables);
    QVERIFY2(tables.contains("users"), "table 'users' should be created during migration");
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(!tables.contains("cars"), "table 'cars' should be dropped during revertMigration");
    QVERIFY2(tables.size() == 3, "one table should be dropped, two (+ migrationTable) should be left");
}

void ApiTest::testUnappliedMigrations()
{
    QSqlMigrator::QSqlMigratorService manager;
    QStringList definedMigrations = manager.definedMigrations(m_context);
    MigrationTracker::SqliteMigrationTableService tableService;
    CommandExecution::CommandExecutionContext serviceContext(m_context.database(), m_context.migrationConfig());
    tableService.addMigration(definedMigrations.at(0), serviceContext);
    tableService.addMigration(definedMigrations.at(2), serviceContext);
    tableService.addMigration(definedMigrations.at(3), serviceContext);

    QStringList unappliedMigrations = manager.unappliedMigrations(m_context);
    QVERIFY2(unappliedMigrations.size() == 1, "one unapplied migration!");
    QVERIFY2(unappliedMigrations.contains(definedMigrations.at(1))
             , "unappliedMigrations should contain second of the defined migrations");
}

QTEST_APPLESS_MAIN(ApiTest)

#include "tst_ApiTest.moc"
