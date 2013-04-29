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

using namespace Structure;
using namespace Migrations;
using namespace MigrationExecution;

BasicTest::BasicTest()
    : m_context(QMap<QString, const Migration*>())
{
}

void BasicTest::testCreateTable()
{
    Commands::CommandPtr command(
                new Commands::CreateTable(
                    Table("testtable1")
                    .add(Column("ID", "int", Column::Primary))
                    .add(Column("name", "varchar(23)", Column::NotNullable))
                    ));

    CommandExecution::CommandExecutionContext serviceContext(*(m_context.database()), m_context.migrationConfig());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository(), serviceContext);

    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1"), "testtable should be created during migration!");
}

void BasicTest::testDropTable()
{
    Commands::CommandPtr command(
                new Commands::CreateTable(
                    Table("testtable1")
                    .add(Column("ID", "int", Column::Primary))
                    .add(Column("name", "varchar(23)", Column::NotNullable))
                    ));

    CommandExecution::CommandExecutionContext serviceContext(*(m_context.database()), m_context.migrationConfig());
    CommandExecution::CommandExecutionService execution;
    execution.execute(command, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository(), serviceContext);

    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1"), "testtable should be created during migration!");

    Commands::CommandPtr command2(
                new Commands::DropTable(
                    Table("testtable1")
                    .add(Column("ID", "int", Column::Primary))
                    .add(Column("name", "varchar(23)", Column::NotNullable))
                    ));

    execution.execute(command2, CommandExecution::CommandExecutionService::Up, m_context.commandServiceRepository(), serviceContext);

    lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(!lTables.contains("testtable1"), "testtable should be droped during migration!");
}

void BasicTest::testTransaction()
{
    if (!m_context.baseMigrationTableService()->canRevertStrucuturalChangesUsingTransactions()) {
        QSKIP("database driver does not support transactions for structural changes, SKIPPING TEST!"
              , SkipSingle);
    }

    Migration m;
    m.add(new Commands::CreateTable(
              Table("testtable1")
              .add(Column("ID", "int", Column::Primary))
              .add(Column("name", "varchar(23)", Column::NotNullable))
              ));

    Migration m2;
    m2.add(new Commands::CreateTable(
               Table("testtable2")
               .add(Column("ID", "int", Column::Primary))
               .add(Column("name", "varchar(23)", Column::NotNullable))
               ));

    m2.add(new Commands::CreateTable(
               Table("testtable1")
               .add(Column("ID", "int", Column::Primary))
               .add(Column("name", "varchar(23)", Column::NotNullable))
               ));

    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;
    MigrationExecutionConfig *migrationConfig = new MigrationExecutionConfig;
    MigrationExecutionContext migrationContext(migrationMap, *migrationConfig);
    migrationContext.setDatabase(m_context.database());
    migrationContext.setBaseMigrationTableService(m_context.baseMigrationTableService());
    migrationContext.setCommandServiceRepository(m_context.commandServiceRepository());

    bool bSuccess = migrator.execute("Migration No1", migrationContext);
    QVERIFY2(bSuccess, "migration should work!");

    bSuccess = migrator.execute("Migration No2", migrationContext);
    QVERIFY2(!bSuccess, "migration should fail!");
    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1")
             , "testtable should be created during migrtaion No 1!");
    QVERIFY2(!lTables.contains("testtable2")
             , "testtable should be deleted during rollback!");
}

void BasicTest::testUndoCreateTable()
{
    if (m_context.baseMigrationTableService()->canRevertStrucuturalChangesUsingTransactions()) {
        QSKIP("database driver should support transactions for structural changes, SKIPPING TEST!"
              , SkipSingle);
    }

    Migration m;
    m.add(new Commands::CreateTable(
              Table("testtable1")
              .add(Column("ID", "int", Column::Primary))
              .add(Column("name", "varchar(23)", Column::NotNullable))
              ));

    Migration m2;
    m2.add(new Commands::CreateTable(
               Table("testtable2")
               .add(Column("ID", "int", Column::Primary))
               .add(Column("name", "varchar(23)", Column::NotNullable))
               ));

    m2.add(new Commands::CreateTable(
               Table("testtable1")
               .add(Column("ID", "int", Column::Primary))
               .add(Column("name", "varchar(23)", Column::NotNullable))
               ));

    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;
    MigrationExecutionConfig *migrationConfig = new MigrationExecutionConfig;
    MigrationExecutionContext migrationContext(migrationMap, *migrationConfig);
    migrationContext.setDatabase(m_context.database());
    migrationContext.setBaseMigrationTableService(m_context.baseMigrationTableService());
    migrationContext.setCommandServiceRepository(m_context.commandServiceRepository());

    bool bSuccess = migrator.execute("Migration No1", migrationContext);
    QVERIFY2(bSuccess, "migration should work!");

    bSuccess = migrator.execute("Migration No2", migrationContext);
    QVERIFY2(!bSuccess, "migration should fail!");
    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1")
             , "testtable should be created during migrtaion No 1!");
    QVERIFY2(!lTables.contains("testtable2")
             , "testtable should be deleted during rollback!");
}

void BasicTest::testUndoDropTable()
{
    if (m_context.baseMigrationTableService()->canRevertStrucuturalChangesUsingTransactions()) {
        QSKIP("database driver should support transactions for structural changes, SKIPPING TEST!"
              , SkipSingle);
    }

    //TODO run dropTable with tableName only to test TableRecovery in DropTable-Command

    Table testtable1 = Table("testtable1")
            .add(Column("ID", "int", Column::Primary))
            .add(Column("name", "varchar(23)", Column::NotNullable));

    Migration m;
    m.add(new Commands::CreateTable(testtable1));

    //TODO add some data to the table to check if undo will reinsert the data..

    Migration m2;
    m2.add(new Commands::DropTable(testtable1));

    m2.add(new Commands::CreateTable(
               Table("testtable2")
               .add(Column("ID", "int", Column::Primary))
               .add(Column("name", "varchar(23)", Column::NotNullable))
               ));

    m2.add(new Commands::CreateTable(
               Table("testtable2")
               .add(Column("ID", "int", Column::Primary))
               .add(Column("name", "varchar(23)", Column::NotNullable))
               ));

    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;
    MigrationExecutionConfig *migrationConfig = new MigrationExecutionConfig;
    MigrationExecutionContext migrationContext(migrationMap, *migrationConfig);
    migrationContext.setDatabase(m_context.database());
    migrationContext.setBaseMigrationTableService(m_context.baseMigrationTableService());
    migrationContext.setCommandServiceRepository(m_context.commandServiceRepository());

    bool bSuccess = migrator.execute("Migration No1", migrationContext);
    QVERIFY2(bSuccess, "migration should work!");

    bSuccess = migrator.execute("Migration No2", migrationContext);
    QVERIFY2(!bSuccess, "migration should fail!");
    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1")
             , "testtable should be re-created during undo!");
    QVERIFY2(!lTables.contains("testtable2")
             , "testtable should be deleted during rollback!");
}

void BasicTest::testMigrationDirections()
{
    Migration m;
    m.add(new Commands::CreateTable(
              Table("testtable1")
              .add(Column("ID", "int", Column::Primary))
              .add(Column("name", "varchar(23)", Column::NotNullable))
              ));

    Migration m2;
    m2.add(new Commands::CreateTable(
               Table("testtable2")
               .add(Column("ID", "int", Column::Primary))
               .add(Column("name", "varchar(23)", Column::NotNullable))
               ));

    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;
    MigrationExecutionConfig *migrationConfig = new MigrationExecutionConfig;
    MigrationExecutionContext migrationContext(migrationMap, *migrationConfig);
    migrationContext.setDatabase(m_context.database());
    migrationContext.setBaseMigrationTableService(m_context.baseMigrationTableService());
    migrationContext.setCommandServiceRepository(m_context.commandServiceRepository());

    bool bSuccess = migrator.execute("Migration No1", migrationContext);
    QVERIFY2(bSuccess, "Migration should work!");
    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1"), "testtable should be created during migration!");

    bSuccess = migrator.execute("Migration No2", migrationContext);
    QVERIFY2(bSuccess, "Migration should work!");
    lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable2"), "testtable should be created during migration!");

    bSuccess = migrator.execute("Migration No2", migrationContext, MigrationExecution::MigrationExecutionService::Down);
    QVERIFY2(bSuccess, "Migration should work!");
    lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(!lTables.contains("testtable2"), "testtable2 should be deleted during migration!");
}

void BasicTest::testDropTableRevert()
{
    Migration m;
    Table table1 = Table("testtable1")
            .add(Column("ID", "int", Column::Primary))
            .add(Column("name", "varchar(23)", Column::NotNullable));
    m.add(new Commands::CreateTable(table1));

    Migration m2;
    m2.add(new Commands::DropTable("testtable1"));
    m2.add(new Commands::DropTable(Table(table1)));    //second DropTable should fail

    MigrationExecutionService migrator;
    QMap<QString, const Migration*> migrationMap;
    migrationMap["Migration No1"] = &m;
    migrationMap["Migration No2"] = &m2;
    MigrationExecutionConfig *migrationConfig = new MigrationExecutionConfig;
    MigrationExecutionContext migrationContext(migrationMap, *migrationConfig);
    migrationContext.setDatabase(m_context.database());
    migrationContext.setBaseMigrationTableService(m_context.baseMigrationTableService());
    migrationContext.setCommandServiceRepository(m_context.commandServiceRepository());

    bool bSuccess = migrator.execute("Migration No1", migrationContext);
    QVERIFY2(bSuccess, "migration should work!");

    QStringList lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1"), "testtable should be created during migration!");

    bSuccess = migrator.execute("Migration No2", migrationContext);
    QVERIFY2(!bSuccess, "migration should fail!");

    lTables = m_context.database()->tables(QSql::Tables);
    QVERIFY2(lTables.contains("testtable1"), "testtable should be recreated during rollback!");
}
