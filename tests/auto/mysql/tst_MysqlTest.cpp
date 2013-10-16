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

    void testCreateIndex();

private:
    QSqlDatabase m_structure_database;
};

MysqlTest::MysqlTest()
{
}

/* Important note: it's not possible to delete a database while connected to it in MySQL,
 * so to delete a database one needs to be connected to another meanwhile.
 * Database MYSQL_STRUCTURE_DATABASE is used for this purpose. */

void MysqlTest::initTestCase()
{
    const QString applicationPath = QCoreApplication::applicationDirPath();
    const QString absoluteApplicationPath = QDir(applicationPath).absolutePath();
    QCoreApplication::addLibraryPath( absoluteApplicationPath ); // wichtig damit die Treiber gefunden werden

    m_structure_database = QSqlDatabase::addDatabase(MYSQL_DRIVERNAME);
    m_structure_database.setHostName(MYSQL_HOSTNAME);
    m_structure_database.setPort(MYSQL_HOSTPORT);
    m_structure_database.setUserName(MYSQL_USERNAME);
    m_structure_database.setPassword(MYSQL_PASSWORD);
    m_structure_database.setDatabaseName(MYSQL_STRUCTURE_DATABASE);

    m_structure_database.open();
    QSqlQuery query;
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(MYSQLTEST_DATABASE_NAME))) {
         ::qDebug() << query.lastError();
    }
    m_structure_database.close();

    ::qDebug() << "running test for MySQL";
}

void MysqlTest::cleanupTestCase()
{
    if (m_context.database().isOpen()) {
        m_context.database().close();
    }

    m_structure_database.database().open();
    QSqlQuery query;
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(MYSQLTEST_DATABASE_NAME))) {
         ::qDebug() << query.lastError();
    }
    m_structure_database.database().close();
}

void MysqlTest::init()
{
    m_structure_database.database().open();
    QSqlQuery query;
    if (!query.exec(QString("CREATE DATABASE %1").arg(MYSQLTEST_DATABASE_NAME))) {
        ::qDebug() << query.lastError();
    }
    m_structure_database.database().close();

    QSqlDatabase database;
    if(!QSqlDatabase::contains("context_connection")) {
        database = QSqlDatabase::addDatabase(MYSQL_DRIVERNAME, "context_connection");
        database.setHostName(MYSQL_HOSTNAME);
        database.setPort(MYSQL_HOSTPORT);
        database.setUserName(MYSQL_USERNAME);
        database.setPassword(MYSQL_PASSWORD);
        database.setDatabaseName(MYSQLTEST_DATABASE_NAME);
    }
    else
        database =  m_context.database();

    bool buildContextSuccess = MysqlMigrator::buildContext(m_context, database);
    QVERIFY2(buildContextSuccess, "context should correctly builded");
}

void MysqlTest::cleanup()
{
    if (m_context.database().isOpen()) {
        m_context.database().close();
    }
    m_structure_database.database().open();
    QSqlQuery query;
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(MYSQLTEST_DATABASE_NAME))) {
         ::qDebug() << query.lastError();
    }
    m_structure_database.database().close();
}

void MysqlTest::testCreateIndex()
{
    base_testCreadeIndex(QString("SHOW INDEXES FROM testtable1"), 2);
}

QTEST_MAIN(MysqlTest)

#include "tst_MysqlTest.moc"
