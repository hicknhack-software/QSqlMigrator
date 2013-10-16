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
#include "PostgresqlMigrator/PostgresqlMigrator.h"

#include "PostgresqlConfig.h"

#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QtTest>

using namespace Structure;
using namespace Migrations;
using namespace MigrationExecution;

#define POSTGRESQL_STRUCTURE_DATABASE "postgresql"
#define POSTGRESQLTEST_DATABASE_NAME "qdb_migrator_test_db"

class PostgresqlTest : public BasicTest
{
    Q_OBJECT

public:
    PostgresqlTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testCreateIndex();

private:
    QSqlDatabase m_structure_database;
};

PostgresqlTest::PostgresqlTest()
{
}

/* Important note: it's not possible to delete a database while connected to it in PostgreSQL,
 * so to delete a database one needs to be connected to another meanwhile.
 * Database POSTGRESQL_STRUCTURE_DATABASE is used for this purpose. */

void PostgresqlTest::initTestCase()
{
    const QString applicationPath = QCoreApplication::applicationDirPath();
    const QString absoluteApplicationPath = QDir(applicationPath).absolutePath();
    QCoreApplication::addLibraryPath( absoluteApplicationPath ); // wichtig damit die Treiber gefunden werden

    m_structure_database = QSqlDatabase::addDatabase(POSTGRESQL_DRIVERNAME);
    m_structure_database.setHostName(POSTGRESQL_HOSTNAME);
    m_structure_database.setPort(POSTGRESQL_HOSTPORT);
    m_structure_database.setUserName(POSTGRESQL_USERNAME);
    m_structure_database.setPassword(POSTGRESQL_PASSWORD);
    m_structure_database.setDatabaseName(POSTGRESQL_STRUCTURE_DATABASE);

    m_structure_database.database().open();
    QSqlQuery query;
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(POSTGRESQLTEST_DATABASE_NAME))) {
        ::qDebug() << query.lastError();
    }
    m_structure_database.database().close();

    ::qDebug() << "running test for PostgreSQL";
}

void PostgresqlTest::cleanupTestCase()
{
    if (m_context.database().isOpen()) {
        m_context.database().close();
    }

    m_structure_database.database().open();
    QSqlQuery query;
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(POSTGRESQLTEST_DATABASE_NAME))) {
         ::qDebug() << query.lastError();
    }
    m_structure_database.database().close();
}

void PostgresqlTest::init()
{
    m_structure_database.database().open();
    QSqlQuery query;
    if (!query.exec(QString("CREATE DATABASE %1").arg(POSTGRESQLTEST_DATABASE_NAME))) {
        ::qDebug() << query.lastError();
    }
    m_structure_database.database().close();

    QSqlDatabase database;
    if(!QSqlDatabase::contains("context_connection")) {
        database = QSqlDatabase::addDatabase(POSTGRESQL_DRIVERNAME, "context_connection");
        database.setHostName(POSTGRESQL_HOSTNAME);
        database.setPort(POSTGRESQL_HOSTPORT);
        database.setUserName(POSTGRESQL_USERNAME);
        database.setPassword(POSTGRESQL_PASSWORD);
        database.setDatabaseName(POSTGRESQLTEST_DATABASE_NAME);
    }
    else
        database =  m_context.database();

    bool buildContextSuccess = PostgresqlMigrator::buildContext(m_context, database);
    QVERIFY2(buildContextSuccess, "context should correctly builded");
}

void PostgresqlTest::cleanup()
{
    if (m_context.database().isOpen()) {
        m_context.database().close();
    }
    m_structure_database.database().open();
    QSqlQuery query;
    if (!query.exec(QString("DROP DATABASE IF EXISTS %1").arg(POSTGRESQLTEST_DATABASE_NAME))) {
         ::qDebug() << query.lastError();
    }
    m_structure_database.database().close();
}

void PostgresqlTest::testCreateIndex()
{
    base_testCreadeIndex(QString("select indexname from pg_indexes where tablename = 'testtable1'"), 0);
}

QTEST_MAIN(PostgresqlTest)

#include "tst_PostgresqlTest.moc"
