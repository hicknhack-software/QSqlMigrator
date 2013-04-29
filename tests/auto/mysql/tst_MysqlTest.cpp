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
#include "../BasicTest/BasicTest.h"
#include "MysqlMigrator/MysqlMigrator.h"
#include "MysqlMigrator/MigrationTracker/MysqlMigrationTableService.h"

#include "MysqlConfig.h"

#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QtTest>

using namespace Structure;
using namespace Migrations;
using namespace MigrationExecution;

#define MYSQL_STRUCTURE_DATABASE "mysql"
#define MYSQLTEST_DATABASE_NAME "qdb_migrator_test_db"

class MysqlTest : public BasicTest
{
    Q_OBJECT

public:
    MysqlTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testAlterColumnType();
    void testCreateIndex();
    void testDropColumn();
    void testRenameColumn();
};

MysqlTest::MysqlTest()
{
}

void MysqlTest::initTestCase()
{
    const QString applicationPath = QCoreApplication::applicationDirPath();
    const QString absoluteApplicationPath = QDir(applicationPath).absolutePath();
    QCoreApplication::addLibraryPath( absoluteApplicationPath ); // wichtig damit die Treiber gefunden werden

    QSqlDatabase *db = new QSqlDatabase;
    *db = QSqlDatabase::addDatabase(MYSQL_DRIVERNAME);
    DatabasePtr sqlDatabase = DatabasePtr(db);
    sqlDatabase->setHostName(MYSQL_HOSTNAME);
    sqlDatabase->setPort(MYSQL_HOSTPORT);
    sqlDatabase->setUserName(MYSQL_USERNAME);
    sqlDatabase->setPassword(MYSQL_PASSWORD);

    CommandServiceRepositoryPtr commandRepository = MysqlMigrator::commandServiceRepository();

    MigrationTableServicePtr migrationTableService =
              MigrationTableServicePtr(new MigrationTracker::MysqlMigrationTableService);

    m_context.setCommandServiceRepository(commandRepository);
    m_context.setBaseMigrationTableService(migrationTableService);
    m_context.setDatabase(sqlDatabase);

    ::qDebug() << "running test for MySQL";
}

void MysqlTest::cleanupTestCase()
{
    m_context.database()->close();
    m_context.database()->setDatabaseName(MYSQL_STRUCTURE_DATABASE);
    m_context.database()->open();
    QSqlQuery query;
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(MYSQLTEST_DATABASE_NAME))) {
         ::qDebug() << query.lastError();
    }
    m_context.database()->close();
}

void MysqlTest::init()
{
    m_context.database()->setDatabaseName(MYSQL_STRUCTURE_DATABASE);
    m_context.database()->open();
    QSqlQuery query;
    if (!query.exec(QString("CREATE DATABASE %1").arg(MYSQLTEST_DATABASE_NAME))) {
         ::qDebug() << query.lastError();
    }
    m_context.database()->close();

    m_context.database()->setDatabaseName(MYSQLTEST_DATABASE_NAME);
    m_context.database()->open();
}

void MysqlTest::cleanup()
{
    m_context.database()->close();
    m_context.database()->setDatabaseName(MYSQL_STRUCTURE_DATABASE);
    m_context.database()->open();
    QSqlQuery query;
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(MYSQLTEST_DATABASE_NAME))) {
         ::qDebug() << query.lastError();
    }
    m_context.database()->close();
}

void MysqlTest::testAlterColumnType()
{
    Commands::CommandPtr command(
                    new Commands::CreateTable(
                         Table("testtable1")
                         .add(Column("ID", "INTEGER", Column::Primary))
                         .add(Column("name", "varchar(23)", Column::NotNullable))
                         .add(Column("col1", "varchar(23)"))
                         .add(Column("col2", "varchar(23)"))
                         ));

    CommandExecution::CommandExecutionContext serviceContext(*(m_context.database()), m_context.migrationConfig());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository() , serviceContext);

    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1"), "testtable should be created during migration!");

    //TODO insert some data

    Commands::CommandPtr command2(
                    new Commands::AlterColumnType("col1", "testtable1", "varchar(42)"));
    execution.execute(command2, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository() , serviceContext);

    //check if old column was removed and new column included successfully
    bool bColumnRetyped = false;
    QSqlQuery query = m_context.database()->exec("DESCRIBE testtable1");
    QSqlError error = query.lastError();
    QVERIFY2(!error.isValid(), "query should run without any error");
    if (error.isValid()) {
         ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
         while (query.next()) {
              QString type = query.value(1).toString();
              if (type == "varchar(42)") {
                    bColumnRetyped = true;
              }
         }
    }
    QVERIFY2(bColumnRetyped, "col1 should be retyped to varchar(42) during migration");

    //TODO check if test data was copied correctly
}

void MysqlTest::testCreateIndex()
{
    Commands::CommandPtr command(
                    new Commands::CreateTable(
                         Table("testtable1")
                         .add(Column("ID", "int", Column::Primary))
                         .add(Column("name", "varchar(23)", Column::NotNullable))
                         .add(Column("col1", "varchar(23)"))
                         .add(Column("col2", "varchar(23)"))
                         ));

    Commands::CommandPtr command2(
                    new Commands::CreateIndex(
                         Index("index1", "testtable1")
                         .addColumn("name", Index::Ascending)
                         .addColumn("col1", Index::Descending)
                         .addColumn("col2")
                         ));

    CommandExecution::CommandExecutionContext serviceContext(*(m_context.database()), m_context.migrationConfig());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository() , serviceContext);
    execution.execute(command2, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository() , serviceContext);

    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1"), "testtable should be created during migration!");

    //check if index was created successfully
    bool bIndexPresent = false;
    QSqlQuery query = m_context.database()->exec("SHOW INDEXES FROM testtable1");
    QSqlError error = query.lastError();
    QVERIFY2(!error.isValid(), "query should run without any error");
    if (error.isValid()) {
         ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
         while (query.next()) {
              QString name = query.value(2).toString();
              if (name == "index1") {
                    bIndexPresent = true;
              }
         }
    }
    QVERIFY2(bIndexPresent, "index1 should be created during CreateIndex");
}

void MysqlTest::testDropColumn()
{
    Commands::CommandPtr command(
                    new Commands::CreateTable(
                         Table("testtable1")
                         .add(Column("ID", "INTEGER", Column::Primary))
                         .add(Column("name", "varchar(23)", Column::NotNullable))
                         .add(Column("col1", "varchar(23)"))
                         .add(Column("col2", "varchar(23)"))
                         ));

    CommandExecution::CommandExecutionContext serviceContext(*(m_context.database()), m_context.migrationConfig());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository() , serviceContext);

    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1"), "testtable should be created during migration!");

    //TODO insert some data

    Commands::CommandPtr command2(
                    new Commands::DropColumn("col1", "testtable1"));
    execution.execute(command2, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository() , serviceContext);

    //check if index was created successfully
    bool bColumnRemoved = false;
    QSqlQuery query = m_context.database()->exec("DESCRIBE testtable1");
    QSqlError error = query.lastError();
    QVERIFY2(!error.isValid(), "query should run without any error");
    if (error.isValid()) {
         ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
         while (query.next()) {
              QString name = query.value(0).toString();
              if (name == "col1") {
                    bColumnRemoved = false;
              }
         }
         bColumnRemoved = true;
    }
    QVERIFY2(bColumnRemoved, "col1 should be removed during migration");

    //TODO check if test data was copied correctly
}

void MysqlTest::testRenameColumn()
{
    Commands::CommandPtr command(
                    new Commands::CreateTable(
                         Table("testtable1")
                         .add(Column("ID", "INTEGER", Column::Primary))
                         .add(Column("name", "varchar(23)", Column::NotNullable))
                         .add(Column("col1", "varchar(23)"))
                         .add(Column("col2", "varchar(23)"))
                         ));

    CommandExecution::CommandExecutionContext serviceContext(*(m_context.database()), m_context.migrationConfig());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository() , serviceContext);

    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1"), "testtable should be created during migration!");

    //TODO insert some data

    Commands::CommandPtr command2(
                    new Commands::RenameColumn("col1", "new_column1", "testtable1"));
    execution.execute(command2, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository() , serviceContext);

    //check if old column was removed and new column included successfully
    bool bColumnRenamed = false;
    QSqlQuery query = m_context.database()->exec("DESCRIBE testtable1");
    QSqlError error = query.lastError();
    QVERIFY2(!error.isValid(), "query should run without any error");
    if (error.isValid()) {
         ::qDebug() << Q_FUNC_INFO << error.text();
    } else {
         while (query.next()) {
              QString name = query.value(0).toString();
              QVERIFY2(name != "col1", "col1 should be removed during migration");
              if (name == "new_column1") {
                    bColumnRenamed = true;
              }
         }
    }
    QVERIFY2(bColumnRenamed, "col1 should be renamed to new_colum1 during migration");

    //TODO check if test data was copied correctly
}

QTEST_MAIN(MysqlTest)

#include "tst_MysqlTest.moc"
