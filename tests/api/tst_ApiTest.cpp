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
#include "M20131501_191807_CreateUsers.h"
#include "M20132201_175827_CreateAddresses.h"
#include "M20132201_180943_CreateCars.h"
#include "M20133001_164323_AddUsers.h"

#include "SqliteMigrator/SqliteMigrator.h"
#include "SqliteMigrator/MigrationTracker/SqliteMigrationTableService.h"
#include "SqliteMigrator/DatabaseLock.h"

#include "LoggingTrace/DebugLogging.h"

#include "SqlDatabaseAdapter/Adapter.h"
#include "SqlMigration/MigrationRepository.h"
#include "SqlMigration/DatabaseMigrationTracker.h"
#include "SqlDatabaseMigrator/SqlMigrator.h"

#include "ApiConfig.h"

#include <QString>
#include <QSqlDatabase>
#include <QtTest>

using namespace QSqlMigrator;
using namespace QSqlMigrator::SqlMigration;

class ApiTest : public QObject {
    Q_OBJECT

public:
    ApiTest();

    SqlMigrator sqlMigrator();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

private:
    void testDatabaseLock();

private Q_SLOTS:
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

private:
    QSharedPointer<LoggingTrace::Logging> m_logging;
    SqlDatabaseAdapter::Adapter m_databaseAdapter;
    SqlDatabaseSchemaAdapter::CommandExecutorRepository m_commandExecutors;
    MigrationRepository m_migrationRepository;
    QSharedPointer<MigrationTracker> m_migrationTracker;
};

ApiTest::ApiTest()
    : m_logging(QSharedPointer<LoggingTrace::DebugLogging>::create())
    , m_commandExecutors(SqliteMigrator::createCommandRepository())
    , m_migrationRepository(MigrationRepository::fromBuilders<M20131501_191807_CreateUsers,
                                                              M20132201_175827_CreateAddresses,
                                                              M20132201_180943_CreateCars,
                                                              M20133001_164323_AddUsers>())
{
}

SqlMigrator
ApiTest::sqlMigrator()
{
    return {{m_migrationRepository,
             m_migrationTracker,
             m_commandExecutors,
             m_databaseAdapter,
             m_logging}};
}

void
ApiTest::initTestCase()
{
    ::qDebug() << "running test for Api";

    QVERIFY2(!m_databaseAdapter.isValid(), "adapter should not be valid!");
    m_databaseAdapter = SqliteMigrator::createAdapter(QSqlDatabase::addDatabase(API_DRIVERNAME));
    QVERIFY2(m_databaseAdapter.isValid(), "adapter should be valid!");

    m_migrationTracker = QSharedPointer<DatabaseMigrationTracker>::create(m_databaseAdapter, m_logging);
}

void
ApiTest::cleanupTestCase()
{
    if (m_databaseAdapter.database().isOpen()) {
        QSqlDatabase(m_databaseAdapter.database()).close();
    }
    if (!QString(API_DATABASE_FILENAME).isEmpty()) {
        if (QFile::exists(API_DATABASE_FILENAME)) {
            QFile::remove(API_DATABASE_FILENAME);
        }
    }
}

void
ApiTest::init()
{
    QSqlDatabase database = m_databaseAdapter.database();
    database.setHostName(API_HOSTNAME);
    database.setPort(API_HOSTPORT);
    database.setDatabaseName(API_DATABASE);
    database.setUserName(API_USERNAME);
    database.setPassword(API_PASSWORD);

    auto successOpen = database.open();
    QVERIFY2(successOpen, "database should open");

    auto successTracker = m_migrationTracker->prepare();
    QVERIFY2(successTracker, "tracker should prepare");
}

void
ApiTest::cleanup()
{
    if (m_databaseAdapter.database().isOpen()) {
        QSqlDatabase(m_databaseAdapter.database()).close();
    }
    if (!QString(API_DATABASE_FILENAME).isEmpty()) {
        QFile::remove(API_DATABASE_FILENAME);
    }
}

void
ApiTest::testDatabaseLock()
{
    auto dummyLockFileName =
        SqliteMigrator::DatabaseLock::buildLockFileName(m_databaseAdapter.database());
    QFile dummyLockFile(dummyLockFileName);
    {
        dummyLockFile.open(QIODevice::WriteOnly);
        dummyLockFile.write("test dummy lock");
        dummyLockFile.close();

        SqliteMigrator::DatabaseLock dummyLockReleaser(m_databaseAdapter.database());
        Q_UNUSED(dummyLockReleaser);
    }
    QVERIFY2(!QFile::exists(dummyLockFileName), "dummy lock file shold NO more exist!");

    {
        SqliteMigrator::DatabaseLock lockA(m_databaseAdapter.database());
        QVERIFY2(lockA, "lockA shold be successful got the lock!");

        SqliteMigrator::DatabaseLock lockB(m_databaseAdapter.database());
        QVERIFY2(!lockB, "lockB should not got the lock, lockA should still have the lock!");
    }

    SqliteMigrator::DatabaseLock lockC(m_databaseAdapter.database());
    QVERIFY2(lockC, "lockC shold be successful got the lock!");
}

void
ApiTest::testRegistrationMacro()
{
    auto migrationNames = m_migrationRepository.migrationNames();
    ::qDebug() << migrationNames;

    QVERIFY2(migrationNames.contains("M20131501_191807_CreateUsers"),
             "migrationList should contain CreateUsers migration");

    QVERIFY2(migrationNames.contains("M20132201_180943_CreateCars"),
             "migrationList should contain CreateCars migration");

    QVERIFY2(migrationNames.contains("M20133001_164323_AddUsers"),
             "migrationList should contain AddUsers migration");

    QVERIFY2(migrationNames.contains("M20132201_175827_CreateAddresses"),
             "migrationList should contain CreateAddresses migration");
}

void
ApiTest::testDefinedMigrations()
{
    auto migrator = sqlMigrator();
    auto migrationNames = migrator.definedMigrations();

    QVERIFY2(migrationNames.contains("M20131501_191807_CreateUsers"),
             "migrationList should contain CreateUsers migration");
    // test if the list was sorted
    QVERIFY2(migrationNames.at(0) == "M20131501_191807_CreateUsers",
             "createUsers must be the first due to its timestamp!");

    QVERIFY2(migrationNames.at(1) == "M20132201_175827_CreateAddresses",
             "createAddresses must be the second due to its timestamp!");

    QVERIFY2(migrationNames.at(2) == "M20132201_180943_CreateCars",
             "createCars must be the third due to its timestamp!");

    QVERIFY2(migrationNames.at(3) == "M20133001_164323_AddUsers",
             "addUsers must be the fourth due to its timestamp!");
}

void
ApiTest::testAppliedMigrations()
{
    auto migrator = sqlMigrator();
    auto definedMigrations = migrator.definedMigrations();
    auto appliedMigrations = migrator.appliedMigrations();
    QVERIFY2(appliedMigrations.size() == 0, "no migration could be applied yet!");

    m_migrationTracker->track(definedMigrations.at(0));

    appliedMigrations = migrator.appliedMigrations();
    QVERIFY2(appliedMigrations.size() == 1, "one migration should be shown as applied");

    QVERIFY2(appliedMigrations.contains(definedMigrations.at(0)),
             "appliedMigrations should contain first of the defined migrations");
}

void
ApiTest::testLastAppliedMigration()
{
    auto migrator = sqlMigrator();

    auto definedMigrations = migrator.definedMigrations();
    auto lastAppliedMigration = migrator.lastAppliedMigration();
    QVERIFY2(lastAppliedMigration.isEmpty(), "no Migration could be applied yet!");

    m_migrationTracker->track(definedMigrations.at(1));

    lastAppliedMigration = migrator.lastAppliedMigration();
    QVERIFY2(lastAppliedMigration == definedMigrations.at(1),
             "second defined migration was applied last!");

    // at this point, 2 migrations are within the migrationTable, but hey are not sorted!
    // nevertheless, the last applied migration has to be the second of the defined migrations
    m_migrationTracker->track(definedMigrations.at(0));

    lastAppliedMigration = migrator.lastAppliedMigration();
    QVERIFY2(lastAppliedMigration == definedMigrations.at(1),
             "second defined migration was applied last!");
}

void
ApiTest::testApplyMigration()
{
    auto migrator = sqlMigrator();

    bool success = migrator.applyMigration("M20132201_175827_CreateAddresses");
    QVERIFY2(success, "applyMigration should return true");

    auto tables = m_databaseAdapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(tables.size() == 2, "only one table (+ migrationTable) should be created");
}

void
ApiTest::testApplyAll()
{
    auto migrator = sqlMigrator();

    bool success = migrator.applyAll();
    QVERIFY2(success, "applyAll should return true");

    auto tables = m_databaseAdapter.database().tables();
    QVERIFY2(tables.contains("users"), "table 'users' should be created during migration");
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(tables.contains("cars"), "table 'cars' should be created during migration");
}

void
ApiTest::testMigrateTo()
{
    auto migrator = sqlMigrator();

    bool success = migrator.migrateTo("M20131501_191807_CreateUsers");
    QVERIFY2(success, "migrateTo should return true");

    auto tables = m_databaseAdapter.database().tables(QSql::Tables);
    QVERIFY2(tables.size() == 2, "one table (+ migrationTable) should be created yet");
    QVERIFY2(tables.contains("users"), "table 'users' should be created during migration");

    success = migrator.migrateTo("M20132201_180943_CreateCars");
    QVERIFY2(success, "migrateTo should return true");
    tables = m_databaseAdapter.database().tables(QSql::Tables);

    QVERIFY2(tables.size() == 4, "two tables should be added during second migrateTo(), makes "
                                 "three (+ migrationTable) tables overall");

    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(tables.contains("cars"), "table 'cars' should be created during migration");
}

void
ApiTest::testMissingMigrations()
{
    auto migrator = sqlMigrator();

    m_migrationTracker->track("M20132301_103512_MissingMigration");

    auto definedMigrations = migrator.definedMigrations();
    auto appliedMigrations = migrator.appliedMigrations();

    QVERIFY2(appliedMigrations.contains("M20132301_103512_MissingMigration"),
             "M20132301_103512_MissingMigration should be part of the appliedMigrationsList");

    QVERIFY2(!definedMigrations.contains("M20132301_103512_MissingMigration"),
             "M20132301_103512_MissingMigration should NOT be part of the definedMigrationsList");

    QStringList missingMigrations = migrator.missingMigrations();
    QVERIFY2(missingMigrations.size() == 1, "one missing migration");
    QVERIFY2(missingMigrations.contains("M20132301_103512_MissingMigration"),
             "M20132301_103512_MissingMigration should be part of the missingMigrationsList");
}

void
ApiTest::testRevertMigration()
{
    auto migrator = sqlMigrator();

    bool success = migrator.applyAll();
    QVERIFY2(success, "applyAll should return true");

    success = migrator.revertMigration("M20132201_180943_CreateCars");
    QVERIFY2(success, "revertMigration should return true");

    auto tables = m_databaseAdapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains("users"), "table 'users' should be created during migration");
    QVERIFY2(tables.contains("addresses"), "table 'addresses' should be created during migration");
    QVERIFY2(!tables.contains("cars"), "table 'cars' should be dropped during revertMigration");
    QVERIFY2(tables.size() == 3,
             "one table should be dropped, two (+ migrationTable) should be left");
}

void
ApiTest::testUnappliedMigrations()
{
    auto migrator = sqlMigrator();

    auto definedMigrations = migrator.definedMigrations();
    m_migrationTracker->track(definedMigrations.at(0));
    m_migrationTracker->track(definedMigrations.at(2));
    m_migrationTracker->track(definedMigrations.at(3));

    auto unappliedMigrations = migrator.unappliedMigrations();
    QVERIFY2(unappliedMigrations.size() == 1, "one unapplied migration!");
    QVERIFY2(unappliedMigrations.contains(definedMigrations.at(1)),
             "unappliedMigrations should contain second of the defined migrations");
}

QTEST_MAIN(ApiTest)

#include "tst_ApiTest.moc"
