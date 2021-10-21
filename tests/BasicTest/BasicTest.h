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
#ifndef BASICTEST_H
#define BASICTEST_H

#include "Commands/AlterColumnType.h"
#include "Commands/CreateIndex.h"
#include "Commands/CreateTable.h"
#include "Commands/DropTable.h"
#include "Commands/DropColumn.h"
#include "Commands/RenameColumn.h"
#include "CommandExecution/BaseCommandExecutionService.h"
#include "CommandExecution/CommandExecutionServiceRepository.h"
#include "MigrationExecution/MigrationExecutionConfig.h"
#include "MigrationExecution/MigrationExecutionContext.h"
#include "MigrationExecution/MigrationExecutionService.h"
#include "Migrations/Migration.h"
#include "MigrationTracker/MigrationTrackerService.h"
#include "Structure/Table.h"
#include "Structure/Column.h"

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
protected Q_SLOTS:
    //TODO add tests for "default value", "autoincrement" and other constraints
    //TODO check if versionTable is maintained correctly
    virtual void initTestCase();
    virtual void cleanupTestCase();
    virtual void init();
    virtual void cleanup();

private Q_SLOTS:
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
    void closeTestDatabase();
    void testLocalSchemeMigration();
    void testCreateIndex();

protected:
    BasicTest(const QString &driverName, const QString &testDatabaseName,
              MigrationExecution::MigrationExecutionContextPtr (*buildContext)(MigrationExecution::MigrationExecutionContext::Builder &),
              const QString &structureDatabase = "", const QString &hostName = "", const int hostPort = 0, const QString &userName = "", const QString &password = "");

protected:
    void initLibraryPath();
    virtual void defineStructureDatabase();
    virtual void createStructureDatabase();
    virtual void cleanStructureDatabase();
    virtual void defineTestDatabase();

    const QString m_driverName;
    QString m_testDatabaseName;
    MigrationExecution::MigrationExecutionContextPtr (*m_buildContext)(MigrationExecution::MigrationExecutionContext::Builder &);

    const QString m_structureDatabase;

    const QString m_hostName;
    const int m_hostPort;
    const QString m_userName;
    const QString m_password;

    MigrationExecution::MigrationExecutionContext::Builder m_contextBuilder;
    MigrationExecution::MigrationExecutionContextPtr m_context;
};

#endif // BASICTEST_H
