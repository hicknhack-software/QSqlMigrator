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
#include "BasicTest.h"

#include "CommandExecution/CommandExecutionContext.h"
#include "CommandExecution/CommandExecutionService.h"

#include "Helper/TypeMapperService.h"
#include "Helper/ColumnService.h"
#include "Helper/SqlStructureService.h"

#include "MigrationExecution/LocalSchemeMigrationExecutionContext.h"
#include "MigrationExecution/LocalSchemeMigrationExecutionService.h"
#include "LocalSchemeMigrator/LocalSchemeMigrator.h"
#include "LocalSchemeMigrator/LocalSchemeComparisonContext.h"
#include "LocalSchemeMigrator/LocalSchemeComparisonService.h"

#include <QScopedPointer>
#include <QSqlQuery>
#include <QSqlError>

#define STRUCTURE_CONNECTION_NAME "structure"

using namespace Structure;
using namespace Migrations;
using namespace MigrationExecution;

BasicTest::BasicTest(const QString &driverName, const QString &testDatabaseName,
                     MigrationExecutionContextPtr (*buildContext)(MigrationExecutionContext::Builder &),
                     const QString &structureDatabase, const QString &hostName, const int hostPort, const QString &userName, const QString &password)
    : m_driverName(driverName)
    , m_testDatabaseName(testDatabaseName)
    , m_buildContext(buildContext)
    , m_structureDatabase(structureDatabase)
    , m_hostName(hostName)
    , m_hostPort(hostPort)
    , m_userName(userName)
    , m_password(password)
    , m_contextBuilder(QMap<QString, const Migration*>())
{
}

/* Important note: it's not possible to delete a database while connected to it in some DBMS
 * so to delete a database one needs to be connected to another meanwhile.
 * Database POSTGRESQL_STRUCTURE_DATABASE is used for this purpose. */

void BasicTest::initLibraryPath()
{
    const QString applicationPath = QCoreApplication::applicationDirPath();
    const QString absoluteApplicationPath = QDir(applicationPath).absolutePath();
    QCoreApplication::addLibraryPath( absoluteApplicationPath ); // wichtig damit die Treiber gefunden werden
}

void BasicTest::defineStructureDatabase()
{
    QSqlDatabase structure_database = QSqlDatabase::addDatabase(m_driverName, STRUCTURE_CONNECTION_NAME);
    structure_database.setHostName(m_hostName);
    structure_database.setPort(m_hostPort);
    structure_database.setUserName(m_userName);
    structure_database.setPassword(m_password);
    structure_database.setDatabaseName(m_structureDatabase);
}

void BasicTest::createStructureDatabase()
{
    QSqlDatabase structure_database = QSqlDatabase::database(STRUCTURE_CONNECTION_NAME);
    QSqlQuery query(structure_database);
    if (!query.exec(QString("CREATE DATABASE %1").arg(m_testDatabaseName))) {
        ::qDebug() << query.lastError();
    }
    structure_database.close();
}

void BasicTest::cleanStructureDatabase()
{
    QSqlDatabase structure_database = QSqlDatabase::database(STRUCTURE_CONNECTION_NAME);
    QSqlQuery query(structure_database);
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(m_testDatabaseName))) {
        ::qDebug() << query.lastError();
    }
    structure_database.close();
}

void BasicTest::defineTestDatabase()
{
    QSqlDatabase test_database = QSqlDatabase::addDatabase(m_driverName, TEST_CONNECTION_NAME);
    test_database.setHostName(m_hostName);
    test_database.setPort(m_hostPort);
    test_database.setUserName(m_userName);
    test_database.setPassword(m_password);
    test_database.setDatabaseName(m_testDatabaseName);
}

void BasicTest::closeTestDatabase()
{
    QSqlDatabase test_database = QSqlDatabase::database(TEST_CONNECTION_NAME, false);
    if (test_database.isOpen()) test_database.close();
    Q_ASSERT(!test_database.isOpen());
}

void BasicTest::initTestCase()
{
    initLibraryPath();
    defineTestDatabase();
    defineStructureDatabase();
    cleanStructureDatabase();

    m_contextBuilder.setDatabase(QSqlDatabase::database(TEST_CONNECTION_NAME, false));
}

void BasicTest::cleanupTestCase()
{
    closeTestDatabase();
    cleanStructureDatabase();
}

void BasicTest::init()
{
    ::qDebug() << "running test for" << m_structureDatabase;

    createStructureDatabase();

    m_context = m_buildContext(m_contextBuilder);
    QVERIFY2(m_context, "context should correctly builded");
}

void BasicTest::cleanup()
{
    closeTestDatabase();
    cleanStructureDatabase();
}

void BasicTest::testCreateTable()
{
    Commands::CommandPtr command(
                new Commands::CreateTable(
                    Table::Builder("testtable1")
                    << Column("ID", Type::Integer, Column::Primary)
                    << Column("name", Type(Type::String, 23), Column::NotNullable)
                    ));

    CommandExecution::CommandExecutionContext serviceContext(m_context->database(), m_context->migrationConfig(), m_context->helperRepository());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, m_context->commandServiceRepository(), serviceContext);

    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");
}

void BasicTest::testDropTable()
{
    Commands::CommandPtr command(
                new Commands::CreateTable(
                    Table::Builder("testtable1")
                    << Column("ID", Type::Integer, Column::Primary)
                    << Column("name", Type(Type::String, 23), Column::NotNullable)
                    ));

    CommandExecution::CommandExecutionContext serviceContext(m_context->database(), m_context->migrationConfig(), m_context->helperRepository());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, m_context->commandServiceRepository(), serviceContext);

    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");

    Commands::CommandPtr command2(
                new Commands::DropTable(
                    Table::Builder("testtable1")
                    << Column("ID", Type::Integer, Column::Primary)
                    << Column("name", Type(Type::String, 23), Column::NotNullable)
                    ));

    execution.execute(command2, m_context->commandServiceRepository(), serviceContext);

    tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(!tables.contains("testtable1"), "testtable should be droped during migration!");
}

void BasicTest::testTransaction()
{
    if (!m_context->migrationTrackerService()->canRevertStrucuturalChangesUsingTransactions()) {
        QSKIP("database driver does not support transactions for structural changes, SKIPPING TEST!", SkipSingle);
    }

    Migration m;
    m.add(new Commands::CreateTable(
              Table::Builder("testtable1")
              << Column("ID", Type::Integer, Column::Primary)
              << Column("name", Type(Type::String, 23), Column::NotNullable)
              ));

    Migration m2;
    m2.add(new Commands::CreateTable(
               Table::Builder("testtable2")
               << Column("ID", Type::Integer, Column::Primary)
               << Column("name", Type(Type::String, 23), Column::NotNullable)
               ));

    m2.add(new Commands::CreateTable(
               Table::Builder("testtable1")
               << Column("ID", Type::Integer, Column::Primary)
               << Column("name", Type(Type::String, 23), Column::NotNullable)
               ));

    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;
    MigrationExecutionContext::Builder migrationContextBuilder(migrationMap);
    migrationContextBuilder.setDatabase(m_context->database());

    MigrationExecutionContextPtr migrationContext = m_buildContext(migrationContextBuilder);

    bool success = migrator.execute("Migration No1", *migrationContext);
    QVERIFY2(success, "migration should work!");

    // migration 'Migration No2' should fail, because table already exists
    QTest::ignoreMessage(QtWarningMsg, "table allready exists! ");
    success = migrator.execute("Migration No2", *migrationContext);
    QVERIFY2(!success, "migration should fail!");
    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migrtaion No 1!");
    QVERIFY2(!tables.contains("testtable2"), "testtable should be deleted during rollback!");
}

void BasicTest::testUndoCreateTable()
{
    if (m_context->migrationTrackerService()->canRevertStrucuturalChangesUsingTransactions()) {
        QSKIP("database driver should support transactions for structural changes, SKIPPING TEST!", SkipSingle);
    }

    Migration m;
    m.add(new Commands::CreateTable(
              Table::Builder("testtable1")
              << Column("ID", Type::Integer, Column::Primary)
              << Column("name", Type(Type::String, 23), Column::NotNullable)
              ));

    Migration m2;
    m2.add(new Commands::CreateTable(
               Table::Builder("testtable2")
               << Column("ID", Type::Integer, Column::Primary)
               << Column("name", Type(Type::String, 23), Column::NotNullable)
               ));

    m2.add(new Commands::CreateTable(
               Table::Builder("testtable1")
               << Column("ID", Type::Integer, Column::Primary)
               << Column("name", Type(Type::String, 23), Column::NotNullable)
               ));

    //QTest::ignoreMessage();
    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;
    MigrationExecutionContext::Builder migrationContextBuilder(migrationMap);
    migrationContextBuilder.setDatabase(m_context->database());

    MigrationExecutionContextPtr migrationContext = m_buildContext(migrationContextBuilder);

    bool success = migrator.execute("Migration No1", *migrationContext);
    QVERIFY2(success, "migration should work!");

    // migration 'Migration No2' should fail, because table 'testtable1' already exists
    QTest::ignoreMessage(QtWarningMsg, "table allready exists! ");
    success = migrator.execute("Migration No2", *migrationContext);
    QVERIFY2(!success, "migration should fail!");
    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration No 1!");
    QVERIFY2(!tables.contains("testtable2"), "testtable should be deleted during rollback!");
}

void BasicTest::testUndoDropTable()
{
    if (m_context->migrationTrackerService()->canRevertStrucuturalChangesUsingTransactions()) {
        QSKIP("database driver should support transactions for structural changes, SKIPPING TEST!", SkipSingle);
    }

    //TODO run dropTable with tableName only to test TableRecovery in DropTable-Command

    Table testtable1( Table::Builder("testtable1")
                      << Column("ID", Type::Integer, Column::Primary)
                      << Column("name", Type(Type::String, 23), Column::NotNullable) );

    Migration m;
    m.add(new Commands::CreateTable(testtable1));

    //TODO add some data to the table to check if undo will reinsert the data..

    Migration m2;
    m2.add(new Commands::DropTable(testtable1));

    m2.add(new Commands::CreateTable(
               Table::Builder("testtable2")
               << Column("ID", Type::Integer, Column::Primary)
               << Column("name", Type(Type::String, 23), Column::NotNullable)
               ));

    m2.add(new Commands::CreateTable(
               Table::Builder("testtable2")
               << Column("ID", Type::Integer, Column::Primary)
               << Column("name", Type(Type::String, 23), Column::NotNullable)
               ));

    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;

    MigrationExecutionContext::Builder migrationContextBuilder(migrationMap);
    migrationContextBuilder.setDatabase(m_context->database());

    MigrationExecutionContextPtr migrationContext = m_buildContext(migrationContextBuilder);

    bool success = migrator.execute("Migration No1", *migrationContext);
    QVERIFY2(success, "migration should work!");

    // migration 'Migration No2' should fail, because table 'testtable2' will be created twice
    QTest::ignoreMessage(QtWarningMsg, "table allready exists! ");
    success = migrator.execute("Migration No2", *migrationContext);
    QVERIFY2(!success, "migration should fail!");
    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be re-created during undo!");
    QVERIFY2(!tables.contains("testtable2"), "testtable should be deleted during rollback!");
}

void BasicTest::testMigrationDirections()
{
    Migration m;
    m.add(new Commands::CreateTable(
              Table::Builder("testtable1")
              << Column("ID", Type::Integer, Column::Primary)
              << Column("name", Type(Type::String, 23), Column::NotNullable)
              ));

    Migration m2;
    m2.add(new Commands::CreateTable(
               Table::Builder("testtable2")
               << Column("ID", Type::Integer, Column::Primary)
               << Column("name", Type(Type::String, 23), Column::NotNullable)
               ));

    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;
    MigrationExecutionContext::Builder migrationContextBuilder(migrationMap);
    migrationContextBuilder.setDatabase(m_context->database());

    MigrationExecutionContextPtr migrationContext = m_buildContext(migrationContextBuilder);

    bool success = migrator.execute("Migration No1", *migrationContext);
    QVERIFY2(success, "Migration should work!");
    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");

    success = migrator.execute("Migration No2", *migrationContext);
    QVERIFY2(success, "Migration should work!");
    tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable2"), "testtable should be created during migration!");

    success = migrator.execute("Migration No2", *migrationContext, MigrationExecution::MigrationExecutionService::Down);
    QVERIFY2(success, "Migration should work!");
    tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(!tables.contains("testtable2"), "testtable2 should be deleted during migration!");
}

void BasicTest::testDropTableRevert()
{
    Migration m;
    Table table1( Table::Builder("testtable1")
                  << Column("ID", Type::Integer, Column::Primary)
                  << Column("name", Type(Type::String, 23), Column::NotNullable) );
    m.add(new Commands::CreateTable(table1));

    Migration m2;
    m2.add(new Commands::DropTable("testtable1"));
    m2.add(new Commands::DropTable(Table(table1)));    //second DropTable should fail

    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;
    MigrationExecutionContext::Builder migrationContextBuilder(migrationMap);
    migrationContextBuilder.setDatabase(m_context->database());

    MigrationExecutionContextPtr migrationContext = m_buildContext(migrationContextBuilder);

    bool success = migrator.execute("Migration No1", *migrationContext);
    QVERIFY2(success, "migration should work!");

    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");

    // migration 'Migration No2' should fail, because table 'testtable1' no longer exists
    QTest::ignoreMessage(QtWarningMsg, "table doesn't exist! ");
    success = migrator.execute("Migration No2", *migrationContext);
    QVERIFY2(!success, "migration should fail!");

    tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be recreated during rollback!");
}

void BasicTest::testAlterColumnType()
{
    Commands::CommandPtr command(
                new Commands::CreateTable(
                    Table::Builder("testtable1")
                    << Column("ID", Type::Integer, Column::Primary)
                    << Column("name", Type(Type::String, 23), Column::NotNullable)
                    << Column("col1", Type(Type::String, 23))
                    << Column("col2", Type(Type::String, 23))
                    ));

    CommandExecution::CommandExecutionContext serviceContext(m_context->database(), m_context->migrationConfig(), m_context->helperRepository());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, m_context->commandServiceRepository(), serviceContext);

    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");

    //TODO insert some data

    //TODO: handle case sensitivity of types and column names
    Commands::CommandPtr command2(new Commands::AlterColumnType("col1", "testtable1", Type(Type::String, 42)));
    execution.execute(command2, m_context->commandServiceRepository(), serviceContext);

    //check if old column was removed and new column included successfully
    Structure::Table table1 = m_context->helperRepository().sqlStructureService().getTableDefinition("testtable1", m_context->database());
    Structure::Column col1( table1.fetchColumnByName("col1") );
    QVERIFY2(col1.isValid(), "column col1 should exist");
    QVERIFY2(0 == QString::compare(col1.type().string(), m_context->helperRepository().typeMapperService().map(Type(Type::String, 42)), Qt::CaseInsensitive), "column col1 should be retyped to varchar(42) during migration");

    Commands::CommandPtr command3(new Commands::AlterColumnType("col1", "testtable1", Type(Type::String, 43)));
    execution.execute(command3, m_context->commandServiceRepository(), serviceContext);

    //check if old column was removed and new column included successfully
    Structure::Table table2 = m_context->helperRepository().sqlStructureService().getTableDefinition("testtable1", m_context->database());
    Structure::Column col2( table2.fetchColumnByName("col1") );
    QVERIFY2(col2.isValid(), "column col1 should exist");
    QVERIFY2(0 == QString::compare(col2.type().string(), m_context->helperRepository().typeMapperService().map(Type(Type::String, 43)), Qt::CaseInsensitive), "column col1 should be retyped to varchar(43) during migration");

    //TODO check if test data was copied correctly
}

void BasicTest::testColumnType()
{
    Table testtable(
                Table::Builder("testtable")
                << Column("bool_",                Type::Boolean)
                << Column("integer_",             Type::Integer)
                << Column("biginteger_",          Type::BigInt)
                << Column("double_",              Type::DoublePrecision)
                << Column("decimal_",             Type(Type::Decimal, 10, 5))
                << Column("date_",                Type::Date)
                << Column("time_",                Type::Time)
                << Column("datetime_",            Type::DateTime)
                << Column("char_1",               Type::Char)
                << Column("char_",                Type(Type::Char, 5))
                << Column("varchar_",             Type(Type::VarChar, 5))
                << Column("blob_",                Type::Binary) );
    Commands::CommandPtr command(new Commands::CreateTable(testtable));

    CommandExecution::CommandExecutionContext serviceContext(m_context->database(), m_context->migrationConfig(), m_context->helperRepository());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, m_context->commandServiceRepository(), serviceContext);

    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains(testtable.name()), "table should be created during migration!");

    Structure::Table table = m_context->helperRepository().sqlStructureService().getTableDefinition(testtable.name(), m_context->database());

    foreach(Structure::Column column, testtable.columns()) {
        Structure::Column col = table.fetchColumnByName(column.name());
        QVERIFY2(col.isValid(), "column should exist");
        if (0 != QString::compare(col.type().string(), m_context->helperRepository().typeMapperService().map(column.type()), Qt::CaseInsensitive)) {
            QCOMPARE(col.type().string(), m_context->helperRepository().typeMapperService().map(column.type()));
        }
    }

}

void BasicTest::testCreateIndex()
{
    Commands::CommandPtr command(
                new Commands::CreateTable(
                    Table::Builder("testtable1")
                    << Column("ID", Type::Integer, Column::Primary)
                    << Column("name", Type(Type::String, 23), Column::NotNullable)
                    << Column("col1", Type(Type::String, 23))
                    << Column("col2", Type(Type::String, 23))
                    ));

    Index index(
                Index::Builder("index1", "testtable1")
                << Index::Column("name", Index::Ascending)
                << Index::Column("col2", m_driverName != "QMYSQL" ? Index::Descending : Index::Ascending) // Mysql does not support index sorting
                << Index::Column("col1")
                );

    Commands::CommandPtr command2(new Commands::CreateIndex(index));

    CommandExecution::CommandExecutionContext serviceContext(m_context->database(), m_context->migrationConfig(), m_context->helperRepository());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, m_context->commandServiceRepository(), serviceContext);
    execution.execute(command2, m_context->commandServiceRepository(), serviceContext);

    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");

    //check if index was created successfully
    Structure::Index realIndex( m_context->helperRepository().sqlStructureService().getIndexDefinition("index1", "testtable1", m_context->database()) );
    bool indexPresent = realIndex.columns() == index.columns();
    if (!indexPresent) {
        qDebug() << "local scheme index:" << m_context->helperRepository().columnService().generateIndexColumnsDefinitionSql(index.columns());
        qDebug() << "real index:" << m_context->helperRepository().columnService().generateIndexColumnsDefinitionSql(realIndex.columns());
    }
    QVERIFY2(indexPresent, "real and local scheme index differ");
}

void BasicTest::testDropColumn()
{
    Commands::CommandPtr command(
                new Commands::CreateTable(
                    Table::Builder("testtable1")
                    << Column("ID", Type::Integer, Column::Primary)
                    << Column("name", Type(Type::String, 23), Column::NotNullable)
                    << Column("col1", Type(Type::String, 23))
                    << Column("col2", Type(Type::String, 23))
                    ));

    CommandExecution::CommandExecutionContext serviceContext(m_context->database(), m_context->migrationConfig(), m_context->helperRepository());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, m_context->commandServiceRepository(), serviceContext);

    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");

    //TODO insert some data

    Commands::CommandPtr command2(new Commands::DropColumn("col1", "testtable1"));
    execution.execute(command2, m_context->commandServiceRepository(), serviceContext);

    //check if column was dropped successfully
    Structure::Column col = m_context->helperRepository().sqlStructureService().getTableDefinition("testtable1", m_context->database()).fetchColumnByName("col1");
    QVERIFY2(!col.isValid(), "col1 should be removed during migration");
}

void BasicTest::testRenameColumn()
{
    Commands::CommandPtr command(
                new Commands::CreateTable(
                    Table::Builder("testtable1")
                    << Column("ID", Type::Integer, Column::Primary)
                    << Column("name", Type(Type::String, 23), Column::NotNullable)
                    << Column("col1", Type(Type::String, 23))
                    << Column("col2", Type(Type::String, 23))
                    ));

    CommandExecution::CommandExecutionContext serviceContext(m_context->database(), m_context->migrationConfig(), m_context->helperRepository());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, m_context->commandServiceRepository(), serviceContext);

    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");

    //TODO insert some data

    Commands::CommandPtr command2(
                new Commands::RenameColumn("col1", "new_column1", "testtable1"));
    execution.execute(command2, m_context->commandServiceRepository(), serviceContext);

    //check if old column was removed and new column included successfully
    Structure::Table table( m_context->helperRepository().sqlStructureService().getTableDefinition("testtable1", m_context->database()) );
    Structure::Column column1( table.fetchColumnByName("col1"));
    QVERIFY2(!column1.isValid(), "col1 should be removed during migration");
    Structure::Column column2( table.fetchColumnByName("new_column1"));
    QVERIFY2(column2.isValid(), "col1 should be renamed to new_colum1 during migration");

    //TODO check if test data was copied correctly
}

void BasicTest::testLocalSchemeMigration()
{
    // migrations
    Migration m;
    m.add(new Commands::CreateTable(
              Table::Builder("testtable1")
              << Column("id1", Type::Integer, Column::Primary | Column::AutoIncrement)
              << Column("name1", Type(Type::String, 23), Column::Unique)
              << Column("weight1", Type::Double)
              ));

    Migration m2;
    m2.add(new Commands::CreateTable(
               Table::Builder("testtable2")
               << Column("id2", Type::Integer, Column::Primary | Column::AutoIncrement)
               << Column("name2", Type(Type::String, 23), Column::Unique)
               << Column("weight2", Type::Double)
               ));
    m2.add(new Commands::CreateIndex(
               Index::Builder("index1", "testtable1")
               << Index::Column("name1")
               << Index::Column("weight1")
               ));

    const QString migrationNo1 = "Migration No1";
    const QString migrationNo2 = "Migration No2";
    QMap<QString, const Migration*> migrationMap;
    migrationMap[migrationNo1] = &m;
    migrationMap[migrationNo2] = &m2;

    bool success;

    // execute migrations on real database
    MigrationExecutionService migrator;
    MigrationExecutionContext::Builder migrationContextBuilder(migrationMap);
    migrationContextBuilder.setDatabase(m_context->database());

    MigrationExecutionContextPtr migrationContext = m_buildContext(migrationContextBuilder);

    success = migrator.execute(migrationNo1, *migrationContext);
    QVERIFY2(success, "Migration should work!");
    QStringList tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");

    success = migrator.execute(migrationNo2, *migrationContext);
    QVERIFY2(success, "Migration should work!");
    tables = m_context->database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable2"), "testtable should be created during migration!");

    // execute migrations on local scheme
    MigrationExecution::LocalSchemeMigrationExecutionService localSchemeMigrator;
    LocalSchemePtr localScheme(new Structure::LocalScheme); // database equivalent
    MigrationExecution::LocalSchemeMigrationExecutionContext localSchemeMigrationContext(migrationMap);
    localSchemeMigrationContext.setLocalScheme(localScheme);
    localSchemeMigrationContext.setLocalSchemeCommandServiceRepository(LocalSchemeMigrator::createCommandServiceRepository());

    success = localSchemeMigrator.execute(migrationNo1, localSchemeMigrationContext);
    QVERIFY2(success, "Migration should work!");
    success = localSchemeMigrator.execute(migrationNo2, localSchemeMigrationContext);
    QVERIFY2(success, "Migration should work!");

    // compare local scheme with database
    LocalSchemeMigrator::LocalSchemeComparisonContext comparisonContext(localScheme, m_context->helperRepository(), m_context->database());
    LocalSchemeMigrator::LocalSchemeComparisonService comparisonService;
    success = comparisonService.compareLocalSchemeWithDatabase(comparisonContext);
    QVERIFY2(success, "local scheme should be identical to actual database scheme");
}
