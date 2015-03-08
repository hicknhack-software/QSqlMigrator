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

#include "LoggingTrace/DebugLogging.h"

#include "SqlDatabaseAdapter/SchemaReflection.h"
#include "SqlDatabaseAdapter/ValueTypeMapper.h"
#include "SqlDatabaseAdapter/ColumnMapper.h"

#include "SqlDatabaseMigrator/MigrationExecutor.h"

#include "SqlMigration/Migration.h"
#include "SqlMigration/DatabaseMigrationTracker.h"

#include "SqlSchemaCommand/CreateTable.h"
#include "SqlSchemaCommand/CreateIndex.h"
#include "SqlSchemaCommand/DropTable.h"
#include "SqlSchemaCommand/AlterColumnType.h"
#include "SqlSchemaCommand/DropColumn.h"
#include "SqlSchemaCommand/RenameColumn.h"

#include <QScopedPointer>
#include <QSqlQuery>
#include <QSqlError>

#define STRUCTURE_CONNECTION_NAME "structure"

using namespace QSqlMigrator;

BasicTest::BasicTest(const QString &driverName,
                     const QString &testDatabaseName,
                     const QString &structureDatabaseName,
                     const QString &hostName,
                     const int hostPort,
                     const QString &userName,
                     const QString &password)
    : m_driverName(driverName)
    , m_testDatabaseName(testDatabaseName)
    , m_structureDatabaseName(structureDatabaseName)
    , m_hostName(hostName)
    , m_hostPort(hostPort)
    , m_userName(userName)
    , m_password(password)
    , m_logging(QSharedPointer<LoggingTrace::DebugLogging>::create())
{
}

/* Important note: it's not possible to delete a database while connected to it in some DBMS
 * so to delete a database one needs to be connected to another meanwhile.
 * Database POSTGRESQL_STRUCTURE_DATABASE is used for this purpose. */

void
BasicTest::initLibraryPath()
{
    const QString applicationPath = QCoreApplication::applicationDirPath();
    const QString absoluteApplicationPath = QDir(applicationPath).absolutePath();
    QCoreApplication::addLibraryPath(
        absoluteApplicationPath); // wichtig damit die Treiber gefunden werden
}

BasicTest::MigrationTracker
BasicTest::migrationTracker()
{
    Q_ASSERT(m_adapter.isValid());
    Q_ASSERT(m_logging);
    return QSharedPointer<SqlMigration::DatabaseMigrationTracker>::create(m_adapter, m_logging);
}

void
BasicTest::defineStructureDatabase()
{
    QSqlDatabase structure_database =
        QSqlDatabase::addDatabase(m_driverName, STRUCTURE_CONNECTION_NAME);
    structure_database.setHostName(m_hostName);
    structure_database.setPort(m_hostPort);
    structure_database.setUserName(m_userName);
    structure_database.setPassword(m_password);
    structure_database.setDatabaseName(m_structureDatabaseName);
}

void
BasicTest::createStructureDatabase()
{
    QSqlDatabase structure_database = QSqlDatabase::database(STRUCTURE_CONNECTION_NAME);
    QSqlQuery query(structure_database);
    if (!query.exec(QString("CREATE DATABASE %1").arg(m_testDatabaseName))) {
        ::qDebug() << query.lastError();
    }
    structure_database.close();
}

void
BasicTest::cleanStructureDatabase()
{
    QSqlDatabase structure_database = QSqlDatabase::database(STRUCTURE_CONNECTION_NAME);
    QSqlQuery query(structure_database);
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(m_testDatabaseName))) {
        ::qDebug() << query.lastError();
    }
    structure_database.close();
}

void
BasicTest::defineTestDatabase()
{
    QSqlDatabase test_database = QSqlDatabase::addDatabase(m_driverName, TEST_CONNECTION_NAME);
    test_database.setHostName(m_hostName);
    test_database.setPort(m_hostPort);
    test_database.setUserName(m_userName);
    test_database.setPassword(m_password);
    test_database.setDatabaseName(m_testDatabaseName);
}

void
BasicTest::closeTestDatabase()
{
    QSqlDatabase test_database = QSqlDatabase::database(TEST_CONNECTION_NAME, false);
    if (test_database.isOpen())
        test_database.close();
}

template <typename Command, typename... Args>
bool
BasicTest::execCommand(Args &&... args)
{
    auto command = QSharedPointer<Command>::create(std::forward<Args>(args)...);
    auto executors = commandExecutors();
    auto executor = executors.findExecutor(command->commandName());
    if (!executor) {
        ::qDebug() << Q_FUNC_INFO << "service not found!" << command->commandName();
        return false;
    }

    auto context = SqlDatabaseSchemaAdapter::CommandExecutorContext(m_adapter, {}, m_logging);
    if (executor->isValid(command, context)) {
        return executor->execute(command, context);
    }
    return false;
}

void
BasicTest::initTestCase()
{
    initLibraryPath();
    defineTestDatabase();
    defineStructureDatabase();
    cleanStructureDatabase();

    m_adapter = buildAdapter(QSqlDatabase::database(TEST_CONNECTION_NAME, false));
    QVERIFY2(m_adapter.isValid(), "adapter should correctly builded");
}

void
BasicTest::cleanupTestCase()
{
    closeTestDatabase();
    cleanStructureDatabase();
}

void
BasicTest::init()
{
    ::qDebug() << "running test for" << m_testDatabaseName;

    createStructureDatabase();

    auto success = m_adapter.database().open();
    QVERIFY2(success, "database should open");
}

void
BasicTest::cleanup()
{
    closeTestDatabase();
    cleanStructureDatabase();
}

void
BasicTest::testCreateTable()
{
#define TEST_TABLE_1 "testtable1"
    using namespace SqlSchema;

    auto success = execCommand<SqlSchemaCommand::CreateTable>(
        TableBuilder(TEST_TABLE_1)
        << Column("ID", ValueType(Type::Integer), Column::Primary)
        << Column("name", ValueType(Type::String, 23), Column::NotNullable));
    QVERIFY2(success, "command should execute");

    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_1), "testtable should be created during migration!");
}

void
BasicTest::testDropTable()
{
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;

    auto success = execCommand<CreateTable>(
        TableBuilder(TEST_TABLE_1)
        << Column("ID", ValueType(Type::Integer), Column::Primary)
        << Column("name", ValueType(Type::String, 23), Column::NotNullable));
    QVERIFY2(success, "command should execute");

    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_1), "testtable should be created during migration!");

    auto success2 =
        execCommand<DropTable>(TableBuilder(TEST_TABLE_1)
                               << Column("ID", ValueType(Type::Integer), Column::Primary)
                               << Column("name", ValueType(Type::String, 23), Column::NotNullable));
    QVERIFY2(success2, "command should execute");

    tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(!tables.contains("testtable1"), "testtable should be droped during migration!");
}

void
BasicTest::testTransaction()
{
    // if (!m_context->migrationTrackerService()->canRevertStructuralChangesUsingTransactions()) {
    QSKIP("database driver does not support transactions for structural changes, SKIPPING TEST!",
          SkipSingle);
    //}

    //    Migration m;
    //    m.add(
    //        new Commands::CreateTable(Table::Builder("testtable1")
    //                                  << Column("ID", Type::Integer, Column::Primary)
    //                                  << Column("name", Type(Type::String, 23),
    //                                  Column::NotNullable)));

    //    Migration m2;
    //    m2.add(
    //        new Commands::CreateTable(Table::Builder("testtable2")
    //                                  << Column("ID", Type::Integer, Column::Primary)
    //                                  << Column("name", Type(Type::String, 23),
    //                                  Column::NotNullable)));

    //    m2.add(
    //        new Commands::CreateTable(Table::Builder("testtable1")
    //                                  << Column("ID", Type::Integer, Column::Primary)
    //                                  << Column("name", Type(Type::String, 23),
    //                                  Column::NotNullable)));

    //    MigrationExecutionService migrator;
    //    QMap<QString, const Migration *> migrationMap;
    //    migrationMap["Migration No1"] = &m;
    //    migrationMap["Migration No2"] = &m2;
    //    MigrationExecutionContext::Builder migrationContextBuilder(migrationMap);
    //    migrationContextBuilder.setDatabase(m_context->database());

    //    MigrationExecutionContextPtr migrationContext = m_buildContext(migrationContextBuilder);

    //    bool success = migrator.execute("Migration No1", *migrationContext);
    //    QVERIFY2(success, "migration should work!");

    //    // migration 'Migration No2' should fail, because table already exists
    //    QTest::ignoreMessage(QtWarningMsg, "table already exists! ");
    //    success = migrator.execute("Migration No2", *migrationContext);
    //    QVERIFY2(!success, "migration should fail!");
    //    QStringList tables = m_context->database().tables(QSql::Tables);
    //    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migrtaion No
    //    1!");
    //    QVERIFY2(!tables.contains("testtable2"), "testtable should be deleted during rollback!");
}

void
BasicTest::testUndoCreateTable()
{
#define TEST_MIGRATION_1 "Migration No1"
#define TEST_MIGRATION_2 "Migration No2"
#define TEST_TABLE_2 "testtable2"
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;
    using namespace SqlMigration;

    auto mb1 = MigrationBuilder(TEST_MIGRATION_1);
    mb1.addCommand<CreateTable>(
        TableBuilder(TEST_TABLE_1)
        << Column("ID", ValueType(Type::Integer), Column::Primary)
        << Column("name", ValueType(Type::String, 23), Column::NotNullable));

    auto mb2 = MigrationBuilder(TEST_MIGRATION_1);
    mb2.addCommand<CreateTable>(
        TableBuilder(TEST_TABLE_2)
        << Column("ID", ValueType(Type::Integer), Column::Primary)
        << Column("name", ValueType(Type::String, 23), Column::NotNullable));

    mb2.addCommand<CreateTable>(
        TableBuilder(TEST_TABLE_1)
        << Column("ID", ValueType(Type::Integer), Column::Primary)
        << Column("name", ValueType(Type::String, 23), Column::NotNullable));

    auto migrations = MigrationRepository{{mb1, mb2}};
    auto setup = SqlDatabaseMigrator::MigrationExecutor::Setup{
        migrations, migrationTracker(), commandExecutors(), m_adapter, m_logging};
    auto executor = SqlDatabaseMigrator::MigrationExecutor{setup};

    auto success = executor.execute(TEST_MIGRATION_1);
    QVERIFY2(success, "migration should work!");

    // migration 'Migration No2' should fail, because table 'testtable1' already exists
    QTest::ignoreMessage(QtWarningMsg, "table already exists! ");
    success = executor.execute(TEST_MIGRATION_2);
    QVERIFY2(!success, "migration should fail!");

    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration No 1!");
    QVERIFY2(!tables.contains("testtable2"), "testtable should be deleted during rollback!");
}

void
BasicTest::testUndoDropTable()
{
    //    if (m_context->migrationTrackerService()->canRevertStructuralChangesUsingTransactions()) {
    //        QSKIP("database driver should support transactions for structural changes, SKIPPING
    //        TEST!",
    //              SkipSingle);
    //    }

    // TODO run dropTable with tableName only to test TableRecovery in DropTable-Command
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;
    using namespace SqlMigration;

    auto testtable1 = TableBuilder(TEST_TABLE_1)
                      << Column("ID", ValueType(Type::Integer), Column::Primary)
                      << Column("name", ValueType(Type::String, 23), Column::NotNullable);

    auto testtable2 = TableBuilder(TEST_TABLE_2)
                      << Column("ID", ValueType(Type::Integer), Column::Primary)
                      << Column("name", ValueType(Type::String, 23), Column::NotNullable);

    auto mb1 = MigrationBuilder(TEST_MIGRATION_1);
    mb1.addCommand<CreateTable>(testtable1);

    auto mb2 = MigrationBuilder(TEST_MIGRATION_1);
    mb2.addCommand<DropTable>(testtable1);
    mb2.addCommand<CreateTable>(testtable2);
    mb2.addCommand<CreateTable>(testtable2);

    auto migrations = MigrationRepository{{mb1, mb2}};
    auto setup = SqlDatabaseMigrator::MigrationExecutor::Setup{
        migrations, migrationTracker(), commandExecutors(), m_adapter, m_logging};
    auto executor = SqlDatabaseMigrator::MigrationExecutor{setup};

    auto success = executor.execute(TEST_MIGRATION_1);
    QVERIFY2(success, "migration should work!");

    // migration 'Migration No2' should fail, because table 'testtable2' will be created twice
    QTest::ignoreMessage(QtWarningMsg, "table already exists! ");
    success = executor.execute(TEST_MIGRATION_2);
    QVERIFY2(!success, "migration should fail!");

    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains("testtable1"), "testtable should be re-created during undo!");
    QVERIFY2(!tables.contains("testtable2"), "testtable should be deleted during rollback!");
}

void
BasicTest::testMigrationDirections()
{
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;
    using namespace SqlMigration;

    auto testtable1 = TableBuilder(TEST_TABLE_1)
                      << Column("ID", ValueType(Type::Integer), Column::Primary)
                      << Column("name", ValueType(Type::String, 23), Column::NotNullable);

    auto testtable2 = TableBuilder(TEST_TABLE_2)
                      << Column("ID", ValueType(Type::Integer), Column::Primary)
                      << Column("name", ValueType(Type::String, 23), Column::NotNullable);

    auto mb1 = MigrationBuilder(TEST_MIGRATION_1);
    mb1.addCommand<CreateTable>(testtable1);

    auto mb2 = MigrationBuilder(TEST_MIGRATION_1);
    mb2.addCommand<CreateTable>(testtable2);

    auto migrations = MigrationRepository{{mb1, mb2}};
    auto setup = SqlDatabaseMigrator::MigrationExecutor::Setup{
        migrations, migrationTracker(), commandExecutors(), m_adapter, m_logging};
    auto executor = SqlDatabaseMigrator::MigrationExecutor{setup};

    auto success = executor.execute(TEST_MIGRATION_1);
    QVERIFY2(success, "migration should work!");
    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_1), "testtable should be created during migration!");

    success = executor.execute(TEST_MIGRATION_2);
    QVERIFY2(success, "migration should work!");
    tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_2), "testtable should be created during migration!");

    success = executor.execute(TEST_MIGRATION_2, SqlDatabaseMigrator::MigrationExecutor::Revert);
    QVERIFY2(success, "Migration should work!");
    tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(!tables.contains(TEST_TABLE_2), "testtable2 should be deleted during migration!");
}

void
BasicTest::testDropTableRevert()
{
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;
    using namespace SqlMigration;

    auto testtable1 = TableBuilder(TEST_TABLE_1)
                      << Column("ID", ValueType(Type::Integer), Column::Primary)
                      << Column("name", ValueType(Type::String, 23), Column::NotNullable);

    auto mb1 = MigrationBuilder(TEST_MIGRATION_1);
    mb1.addCommand<CreateTable>(testtable1);

    auto mb2 = MigrationBuilder(TEST_MIGRATION_2);
    mb2.addCommand<DropTable>(TEST_TABLE_1);
    mb2.addCommand<DropTable>(testtable1); // second DropTable should fail

    auto migrations = MigrationRepository{{mb1, mb2}};
    auto setup = SqlDatabaseMigrator::MigrationExecutor::Setup{
        migrations, migrationTracker(), commandExecutors(), m_adapter, m_logging};
    auto executor = SqlDatabaseMigrator::MigrationExecutor{setup};

    auto success = executor.execute(TEST_MIGRATION_1);
    QVERIFY2(success, "migration should work!");
    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_1), "testtable should be created during migration!");

    // migration 'Migration No2' should fail, because table 'testtable1' no longer exists
    QTest::ignoreMessage(QtWarningMsg, "table doesn't exist! ");
    success = executor.execute(TEST_MIGRATION_2);
    QVERIFY2(!success, "migration should fail!");

    tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_1), "testtable should be recreated during rollback!");
}

void
BasicTest::testAlterColumnType()
{
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;
#define TEST_COLUMN_1 "col1"
#define TEST_VALUE_1 42
#define TEST_VALUE_2 43
    auto success =
        execCommand<CreateTable>(TableBuilder(TEST_TABLE_1)
                                 << Column("ID", ValueType(Type::Integer), Column::Primary)
                                 << Column("name", ValueType(Type::String, 23), Column::NotNullable)
                                 << Column(TEST_COLUMN_1, ValueType(Type::String, 23))
                                 << Column("col2", ValueType(Type::String, 23)));
    QVERIFY2(success, "command should execute");

    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_1), "testtable should be created during migration!");

    // TODO insert some data

    // TODO: handle case sensitivity of types and column names
    success = execCommand<AlterColumnType>(TEST_TABLE_1, TEST_COLUMN_1,
                                           ValueType(Type::String, TEST_VALUE_1));
    QVERIFY2(success, "command should execute");

    // check if old column was removed and new column included successfully
    auto table1 =
        m_adapter.schemaReflection()->getTableDefinition(TEST_TABLE_1, m_adapter.database());
    auto col1 = table1.column(TEST_COLUMN_1);
    QVERIFY2(col1.isValid(), "column col1 should exist");
    QVERIFY2(0 == QString::compare(col1.type().string(), m_adapter.valueTypeMapper()->map(
                                                             ValueType(Type::String, TEST_VALUE_1)),
                                   Qt::CaseInsensitive),
             "column col1 should be retyped to varchar(42) during migration");

    success = execCommand<AlterColumnType>(TEST_TABLE_1, TEST_COLUMN_1,
                                           ValueType(Type::String, TEST_VALUE_2));
    QVERIFY2(success, "command should execute");

    // check if old column was removed and new column included successfully
    table1 = m_adapter.schemaReflection()->getTableDefinition(TEST_TABLE_1, m_adapter.database());
    col1 = table1.column(TEST_COLUMN_1);
    QVERIFY2(col1.isValid(), "column col1 should exist");
    QVERIFY2(0 == QString::compare(col1.type().string(), m_adapter.valueTypeMapper()->map(
                                                             ValueType(Type::String, TEST_VALUE_2)),
                                   Qt::CaseInsensitive),
             "column col1 should be retyped to varchar(42) during migration");

    // TODO check if test data was copied correctly
}

void
BasicTest::testColumnType()
{
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;

    Table testtable = TableBuilder("testtable")
                      << Column("bool_", ValueType(Type::Boolean))
                      << Column("integer_", ValueType(Type::Integer))
                      << Column("biginteger_", ValueType(Type::BigInt))
                      << Column("double_", ValueType(Type::DoublePrecision))
                      << Column("decimal_", ValueType(Type::Decimal, 10, 5))
                      << Column("date_", ValueType(Type::Date))
                      << Column("time_", ValueType(Type::Time))
                      << Column("datetime_", ValueType(Type::DateTime))
                      << Column("char_1", ValueType(Type::Char))
                      << Column("char_", ValueType(Type::Char, 5))
                      << Column("varchar_", ValueType(Type::VarChar, 5))
                      << Column("blob_", ValueType(Type::Binary));

    auto success = execCommand<CreateTable>(testtable);
    QVERIFY2(success, "command should execute");

    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(testtable.name()), "table should be created during migration!");

    auto table =
        m_adapter.schemaReflection()->getTableDefinition(testtable.name(), m_adapter.database());

    foreach (Column column, testtable.columns()) {
        Column col = table.column(column.name());
        QVERIFY2(col.isValid(), "column should exist");
        if (0 != QString::compare(col.type().string(),
                                  m_adapter.valueTypeMapper()->map(column.type()),
                                  Qt::CaseInsensitive)) {
            QCOMPARE(col.type().string(), m_adapter.valueTypeMapper()->map(column.type()));
        }
    }
}

void
BasicTest::testCreateIndex()
{
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;
#define TEST_COLUMN_2 "col2"
#define TEST_COLUMN_3 "name"
#define TEST_INDEX_1 "index1"

    auto success = execCommand<CreateTable>(
        TableBuilder(TEST_TABLE_1)
        << Column("ID", ValueType(Type::Integer), Column::Primary)
        << Column(TEST_COLUMN_1, ValueType(Type::String, 23))
        << Column(TEST_COLUMN_2, ValueType(Type::String, 23))
        << Column(TEST_COLUMN_3, ValueType(Type::String, 23), Column::NotNullable));
    QVERIFY2(success, "command should execute");

    Index index = IndexBuilder(TEST_INDEX_1, TEST_TABLE_1)
                  << IndexColumn(TEST_COLUMN_3, SortOrder::Ascending)
                  << IndexColumn(TEST_COLUMN_2,
                                 m_driverName != "QMYSQL"
                                     ? SortOrder::Descending
                                     : SortOrder::Ascending) // Mysql does not support index sorting
                  << IndexColumn(TEST_COLUMN_1);

    success = execCommand<CreateIndex>(index);
    QVERIFY2(success, "command should execute");

    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_1), "testtable should be created during migration!");

    // check if index was created successfully
    auto realIndex = m_adapter.schemaReflection()->getIndexDefinition(TEST_INDEX_1, TEST_TABLE_1,
                                                                      m_adapter.database());

    bool indexPresent = realIndex.columns() == index.columns();
    if (!indexPresent) {
        qDebug() << "local scheme index:"
                 << m_adapter.columnMapper()->generateIndexColumnsDefinitionSql(index.columns());
        qDebug() << "real index:"
                 << m_adapter.columnMapper()->generateIndexColumnsDefinitionSql(
                        realIndex.columns());
    }
    QVERIFY2(indexPresent, "real and local scheme index differ");
}

void
BasicTest::testDropColumn()
{
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;

    auto success = execCommand<CreateTable>(
        TableBuilder(TEST_TABLE_1)
        << Column("ID", ValueType(Type::Integer), Column::Primary)
        << Column(TEST_COLUMN_1, ValueType(Type::String, 23))
        << Column(TEST_COLUMN_2, ValueType(Type::String, 23))
        << Column(TEST_COLUMN_3, ValueType(Type::String, 23), Column::NotNullable));
    QVERIFY2(success, "command should execute");
    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_1), "testtable should be created during migration!");

    // TODO insert some data

    success = execCommand<DropColumn>(TEST_TABLE_1, TEST_COLUMN_1);
    QVERIFY2(success, "command should execute");

    // check if column was dropped successfully
    Column col = m_adapter.schemaReflection()
                     ->getTableDefinition(TEST_TABLE_1, m_adapter.database())
                     .column(TEST_COLUMN_1);
    QVERIFY2(!col.isValid(), "col1 should be removed during migration");
}

void
BasicTest::testRenameColumn()
{
    using namespace SqlSchema;
    using namespace SqlSchemaCommand;

    auto success = execCommand<CreateTable>(
        TableBuilder(TEST_TABLE_1) << Column("ID", ValueType(Type::Integer), Column::Primary)
                                   << Column(TEST_COLUMN_1, ValueType(Type::String, 23)));
    QVERIFY2(success, "command should execute");
    auto tables = m_adapter.database().tables(QSql::Tables);
    QVERIFY2(tables.contains(TEST_TABLE_1), "testtable should be created during migration!");

    // TODO insert some data

    success = execCommand<RenameColumn>(TEST_TABLE_1, TEST_COLUMN_1, TEST_COLUMN_2);
    QVERIFY2(success, "command should execute");

    // check if old column was removed and new column included successfully
    auto table1 =
        m_adapter.schemaReflection()->getTableDefinition(TEST_TABLE_1, m_adapter.database());
    auto col1 = table1.column(TEST_COLUMN_1);
    QVERIFY2(!col1.isValid(), "col1 should be removed during migration");
    auto col2 = table1.column(TEST_COLUMN_2);
    QVERIFY2(col2.isValid(), "col1 should be renamed to new_colum1 during migration");

    // TODO check if test data was copied correctly
}

void
BasicTest::testLocalSchemeMigration()
{
    // migrations
    //    Migration m;
    //    m.add(new Commands::CreateTable(
    //        Table::Builder("testtable1")
    //        << Column("id1", Type::Integer, Column::Primary | Column::AutoIncrement)
    //        << Column("name1", Type(Type::String, 23), Column::Unique)
    //        << Column("weight1", Type::Double)));

    //    Migration m2;
    //    m2.add(new Commands::CreateTable(
    //        Table::Builder("testtable2")
    //        << Column("id2", Type::Integer, Column::Primary | Column::AutoIncrement)
    //        << Column("name2", Type(Type::String, 23), Column::Unique)
    //        << Column("weight2", Type::Double)));
    //    m2.add(new Commands::CreateIndex(Index::Builder("index1", "testtable1")
    //                                     << Index::Column("name1") << Index::Column("weight1")));

    //    const QString migrationNo1 = "Migration No1";
    //    const QString migrationNo2 = "Migration No2";
    //    QMap<QString, const Migration *> migrationMap;
    //    migrationMap[migrationNo1] = &m;
    //    migrationMap[migrationNo2] = &m2;

    //    bool success;

    //    // execute migrations on real database
    //    MigrationExecutionService migrator;
    //    MigrationExecutionContext::Builder migrationContextBuilder(migrationMap);
    //    migrationContextBuilder.setDatabase(m_context->database());

    //    MigrationExecutionContextPtr migrationContext = m_buildContext(migrationContextBuilder);

    //    success = migrator.execute(migrationNo1, *migrationContext);
    //    QVERIFY2(success, "Migration should work!");
    //    QStringList tables = m_context->database().tables(QSql::Tables);
    //    QVERIFY2(tables.contains("testtable1"), "testtable should be created during migration!");

    //    success = migrator.execute(migrationNo2, *migrationContext);
    //    QVERIFY2(success, "Migration should work!");
    //    tables = m_context->database().tables(QSql::Tables);
    //    QVERIFY2(tables.contains("testtable2"), "testtable should be created during migration!");

    //    // execute migrations on local scheme
    //    MigrationExecution::LocalSchemeMigrationExecutionService localSchemeMigrator;
    //    LocalSchemePtr localScheme(new Structure::LocalScheme); // database equivalent
    //    MigrationExecution::LocalSchemeMigrationExecutionContext localSchemeMigrationContext(
    //        migrationMap);
    //    localSchemeMigrationContext.setLocalScheme(localScheme);
    //    localSchemeMigrationContext.setLocalSchemeCommandServiceRepository(
    //        LocalSchemeMigrator::createCommandServiceRepository());

    //    success = localSchemeMigrator.execute(migrationNo1, localSchemeMigrationContext);
    //    QVERIFY2(success, "Migration should work!");
    //    success = localSchemeMigrator.execute(migrationNo2, localSchemeMigrationContext);
    //    QVERIFY2(success, "Migration should work!");

    //    // compare local scheme with database
    //    LocalSchemeMigrator::LocalSchemeComparisonContext comparisonContext(
    //        localScheme, m_context->helperRepository(), m_context->database());
    //    LocalSchemeMigrator::LocalSchemeComparisonService comparisonService;
    //    success = comparisonService.compareLocalSchemeWithDatabase(comparisonContext);
    //    QVERIFY2(success, "local scheme should be identical to actual database scheme");
}
