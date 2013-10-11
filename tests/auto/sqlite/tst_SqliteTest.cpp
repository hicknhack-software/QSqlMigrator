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
#include "SqliteMigrator/SqliteMigrator.h"

#include "SqliteConfig.h"

#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QtTest>

using namespace Structure;
using namespace Migrations;
using namespace MigrationExecution;

class SqliteTest : public BasicTest
{
    Q_OBJECT

public:
    SqliteTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testCreateIndex();
};

SqliteTest::SqliteTest()
{
}

void SqliteTest::initTestCase()
{
    ::qDebug() << "running test for SQLite";
}

void SqliteTest::cleanupTestCase()
{
    if (m_context.database().isOpen()) {
        m_context.database().close();
    }
    if (QFile::exists(SQLITE_DATABASE_FILE)) {
        QFile::remove(SQLITE_DATABASE_FILE);
    }
}

void SqliteTest::init()
{
    QSqlDatabase database;
    if(!QSqlDatabase::contains(/*default-connection*/)) {
        database = QSqlDatabase::addDatabase(SQLITE_DRIVERNAME);
        database.setDatabaseName(SQLITE_DATABASE_FILE);
    }
    else
        database = m_context.database();

    bool success = SqliteMigrator::buildContext(m_context, database);
    QVERIFY2(success, "context should be builded correctly!");
}

void SqliteTest::cleanup()
{
    if (m_context.database().isOpen()) {
        m_context.database().close();
    }
    QFile::remove(SQLITE_DATABASE_FILE);
}

void SqliteTest::testCreateIndex()
{
    base_testCreadeIndex(QString("PRAGMA index_list(testtable1)"), 1);
}

QTEST_APPLESS_MAIN(SqliteTest)

#include "tst_SqliteTest.moc"
