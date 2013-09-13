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

ApiTest::ApiTest()
    : m_context(MigrationRepository::migrations(), MigrationExecutionConfig())
{
}


void ApiTest::initTestCase()
{
    //TODO der gesamte Kontext kann automatish aus der DB-Config erstellt werden.
    //dazu müssen ein entsprechendes Objekt und ein zugehöriger Service erstellt werden

    QSqlDatabase database = QSqlDatabase::addDatabase(API_DRIVERNAME);
    database.setDatabaseName(API_DATABASE);

    CommandServiceRepositoryPtr commandRepository = SqliteMigrator::commandServiceRepository();
    MigrationTableServicePtr migrationTableService(new MigrationTracker::SqliteMigrationTableService);

    m_context.setCommandServiceRepository(commandRepository);
    m_context.setBaseMigrationTableService(migrationTableService);
    m_context.setDatabase(database);
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
    m_context.database().open();

    MigrationTableServicePtr migrationTableService = m_context.baseMigrationTableService();
    CommandExecution::CommandExecutionContext serviceContext(m_context.database(), m_context.migrationConfig());
    QVERIFY2(migrationTableService->ensureVersionTable(serviceContext), "MigrationVersionTable should be created");
}

void ApiTest::cleanup()
{
    m_context.database().close();
    if (!QString(API_DATABASE_FILENAME).isEmpty()) {
        QFile::remove(API_DATABASE_FILENAME);
    }
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
    QStringList sMigrations = manager.definedMigrations(m_context);
    QVERIFY2(sMigrations.contains("M20131501_191807_CreateUsers")
             , "migrationList should contain CreateUsers migration");
    //test if the list was sorted
    QVERIFY2(sMigrations.at(0) == "M20131501_191807_CreateUsers"
             , "createUsers must be the first due to its timestamp!");
    QVERIFY2(sMigrations.at(1) == "M20132201_175827_CreateAddresses"
             , "createAddresses must be the second due to its timestamp!");
    QVERIFY2(sMigrations.at(2) == "M20132201_180943_CreateCars"
             , "createCars must be the third due to its timestamp!");
    QVERIFY2(sMigrations.at(3) == "M20133001_164323_AddUsers"
             , "addUsers must be the fourth due to its timestamp!");
}

void ApiTest::testAppliedMigrations()
{
    QSqlMigrator::QSqlMigratorService manager;
    QStringList sDefinedMigrations = manager.definedMigrations(m_context);
    QStringList sAppliedMigrations = manager.appliedMigrations(m_context);
    QVERIFY2(sAppliedMigrations.size() == 0, "no migration could be applied yet!");

    MigrationTracker::SqliteMigrationTableService tableService;
    CommandExecution::CommandExecutionContext serviceContext(m_context.database(), m_context.migrationConfig());
    tableService.addMigration(sDefinedMigrations.at(0), serviceContext);
    sAppliedMigrations = manager.appliedMigrations(m_context);
    QVERIFY2(sAppliedMigrations.size() == 1, "one migration should be shown as applied");
    QVERIFY2(sAppliedMigrations.contains(sDefinedMigrations.at(0))
             , "appliedMigrations should contain first of the defined migrations");
}

void ApiTest::testLastAppliedMigration()
{
    QSqlMigrator::QSqlMigratorService manager;
    QStringList sDefinedMigrations = manager.definedMigrations(m_context);
    QString sLastAppliedMigration = manager.lastAppliedMigration(m_context);
    QVERIFY2(sLastAppliedMigration.isEmpty(), "no Migration could be applied yet!");

    MigrationTracker::SqliteMigrationTableService tableService;
    CommandExecution::CommandExecutionContext serviceContext(m_context.database(), m_context.migrationConfig());
    tableService.addMigration(sDefinedMigrations.at(1), serviceContext);
    sLastAppliedMigration = manager.lastAppliedMigration(m_context);
    QVERIFY2(sLastAppliedMigration == sDefinedMigrations.at(1), "second defined migration was applied last!");

    //at this point, 2 migrations are within the migrationTable, but hey are not sorted! nevertheless,
    //the last applied migration has to be the second of the defined migrations
    tableService.addMigration(sDefinedMigrations.at(0), serviceContext);
    sLastAppliedMigration = manager.lastAppliedMigration(m_context);
    QVERIFY2(sLastAppliedMigration == sDefinedMigrations.at(1), "second defined migration was applied last!");
}

void ApiTest::testApplyMigration()
{
    QSqlMigrator::QSqlMigratorService manager;
    bool bSuccess = manager.applyMigration("M20132201_175827_CreateAddresses", m_context);
    QVERIFY2(bSuccess, "applyMigration should return true");
    QStringList tables = m_context.database().tables(QSql::Tables);
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(tables.size() == 2, "only one table (+ migrationTable) should be created");
}

void ApiTest::testApplyAll()
{
    QSqlMigrator::QSqlMigratorService manager;
    bool bSuccess = manager.applyAll(m_context);
    QVERIFY2(bSuccess, "applyAll should return true");
    QStringList tables = m_context.database().tables();
    QVERIFY2(tables.contains("users"), "table 'users' should be created during migration");
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(tables.contains("cars"), "table 'cars' should be created during migration");

    QFile::copy(API_DATABASE_FILENAME, "vorher.sqlite3");
}

void ApiTest::testMigrateTo()
{
    QSqlMigrator::QSqlMigratorService manager;
    bool bSuccess = manager.migrateTo("M20131501_191807_CreateUsers", m_context);
    QVERIFY2(bSuccess, "migrateTo should return true");
    QStringList tables = m_context.database().tables(QSql::Tables);
    QVERIFY2(tables.size() == 2, "one table (+ migrationTable) should be created yet");
    QVERIFY2(tables.contains("users"), "table 'users' should be created during migration");
    bSuccess = manager.migrateTo("M20132201_180943_CreateCars", m_context);
    QVERIFY2(bSuccess, "migrateTo should return true");
    tables = m_context.database().tables(QSql::Tables);
    QVERIFY2(tables.size() == 4
             , "two tables should be added during second migrateTo(), makes three (+ migrationTable) tables overall");
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(tables.contains("cars"), "table 'cars' should be created during migration");

    QFile::copy(API_DATABASE_FILENAME, "hier.sqlite3");

}

void ApiTest::testMissingMigrations()
{
    QSqlMigrator::QSqlMigratorService manager;
    MigrationTracker::SqliteMigrationTableService tableService;
    CommandExecution::CommandExecutionContext serviceContext(m_context.database(), m_context.migrationConfig());
    tableService.addMigration("M20132301_103512_MissingMigration", serviceContext);
    QStringList sDefinedMigrations = manager.definedMigrations(m_context);
    QStringList sAppliedMigrations = manager.appliedMigrations(m_context);
    QVERIFY2(sAppliedMigrations.contains("M20132301_103512_MissingMigration")
             , "M20132301_103512_MissingMigration should be part of the appliedMigrationsList");
    QVERIFY2(!sDefinedMigrations.contains("M20132301_103512_MissingMigration")
             , "M20132301_103512_MissingMigration should NOT be part of the definedMigrationsList");
    QStringList sMissingMigrations = manager.missingMigrations(m_context);
    QVERIFY2(sMissingMigrations.size() == 1, "one missing migration");
    QVERIFY2(sMissingMigrations.contains("M20132301_103512_MissingMigration")
             , "M20132301_103512_MissingMigration should be part of the missingMigrationsList");
}

void ApiTest::testRevertMigration()
{
    QSqlMigrator::QSqlMigratorService manager;
    bool bSuccess = manager.applyAll(m_context);
    QVERIFY2(bSuccess, "applyAll should return true");
    bSuccess = manager.revertMigration("M20132201_180943_CreateCars", m_context);
    QVERIFY2(bSuccess, "revertMigration should return true");
    QStringList tables = m_context.database().tables(QSql::Tables);
    QVERIFY2(tables.contains("users"), "table 'users' should be created during migration");
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(!tables.contains("cars"), "table 'cars' should be dropped during revertMigration");
    QVERIFY2(tables.size() == 3, "one table should be dropped, two (+ migrationTable) should be left");
}

void ApiTest::testUnappliedMigrations()
{
    QSqlMigrator::QSqlMigratorService manager;
    QStringList sDefinedMigrations = manager.definedMigrations(m_context);
    MigrationTracker::SqliteMigrationTableService tableService;
    CommandExecution::CommandExecutionContext serviceContext(m_context.database(), m_context.migrationConfig());
    tableService.addMigration(sDefinedMigrations.at(0), serviceContext);
    tableService.addMigration(sDefinedMigrations.at(2), serviceContext);
    tableService.addMigration(sDefinedMigrations.at(3), serviceContext);

    QStringList sUnappliedMigrations = manager.unappliedMigrations(m_context);
    QVERIFY2(sUnappliedMigrations.size() == 1, "one unapplied migration!");
    QVERIFY2(sUnappliedMigrations.contains(sDefinedMigrations.at(1))
             , "unappliedMigrations should contain second of the defined migrations");
}

QTEST_APPLESS_MAIN(ApiTest)

#include "tst_ApiTest.moc"
