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
#include <QtTest>
#include <QSqlQuery>
#include <QFile>
#include <QSqlError>

using namespace Structure;
using namespace Migrations;
using namespace MigrationExecution;

class SqliteTest : public BasicTest
{
    Q_OBJECT

public:
    SqliteTest();

private:
    void defineStructureDatabase() override;
    void createStructureDatabase() override;
    void cleanStructureDatabase() override;
    void defineTestDatabase() override;
    void init() override;
    void initTestCase() override;

};

SqliteTest::SqliteTest() : BasicTest(SQLITE_DRIVERNAME, SQLITE_DATABASE_FILE, &SqliteMigrator::buildContext)
{
}

void SqliteTest::initTestCase()
{
    initLibraryPath();
    defineTestDatabase();
    defineStructureDatabase();

    m_contextBuilder.setDatabase(QSqlDatabase::database(TEST_CONNECTION_NAME, false));
}

void SqliteTest::init() {
    auto getFilename = []() -> QString {
        QTemporaryFile tempFile;
        Q_ASSERT(tempFile.open());
        return tempFile.fileName();
    };
    m_testDatabaseName = getFilename();
    qDebug() << "Use DatabaseName " << m_testDatabaseName;
    defineTestDatabase();
    m_contextBuilder.setDatabase(QSqlDatabase::database(TEST_CONNECTION_NAME, false));
    BasicTest::init();
}

void SqliteTest::defineStructureDatabase()
{
    // empty
}

void SqliteTest::createStructureDatabase()
{
    // workaround for NFS mount bug present on Github Actions
    QSqlDatabase test_database = QSqlDatabase::database(TEST_CONNECTION_NAME);
    auto query = test_database.exec("SELECT 1");
    if (query.lastError().isValid()) {
        ::qDebug() << "initial query error";
    }
    test_database.close();
    Q_ASSERT(QFile::exists(m_testDatabaseName));
}

void SqliteTest::cleanStructureDatabase()
{
    QFile file(m_testDatabaseName);
    if (file.exists()) {
        Q_ASSERT_X(file.remove(), m_testDatabaseName.toStdString().c_str(), file.errorString().toStdString().c_str()) ;
    }
    Q_ASSERT(!QFile::exists(m_testDatabaseName));
}

void SqliteTest::defineTestDatabase()
{
    QSqlDatabase database = QSqlDatabase::addDatabase(m_driverName, TEST_CONNECTION_NAME);
    database.setDatabaseName(m_testDatabaseName);
}

QTEST_MAIN(SqliteTest)

#include "tst_SqliteTest.moc"
