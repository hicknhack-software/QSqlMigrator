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

class SqliteTest : public BasicTest {
    Q_OBJECT

public:
    SqliteTest();

protected:
    Adapter buildAdapter(QSqlDatabase) Q_DECL_OVERRIDE;
    CommandExecutors commandExecutors() Q_DECL_OVERRIDE;

private:
    void defineStructureDatabase() Q_DECL_OVERRIDE;
    void createStructureDatabase() Q_DECL_OVERRIDE;
    void cleanStructureDatabase() Q_DECL_OVERRIDE;
    void defineTestDatabase() Q_DECL_OVERRIDE;
};

SqliteTest::SqliteTest()
    : BasicTest(SQLITE_DRIVERNAME, SQLITE_DATABASE_FILE)
{
}

BasicTest::Adapter
SqliteTest::buildAdapter(QSqlDatabase database)
{
    return SqliteMigrator::createAdapter(database);
}

BasicTest::CommandExecutors
SqliteTest::commandExecutors()
{
    return SqliteMigrator::createCommandRepository();
}

void
SqliteTest::defineStructureDatabase()
{
    // empty
}

void
SqliteTest::createStructureDatabase()
{
    // empty
}

void
SqliteTest::cleanStructureDatabase()
{
    if (QFile::exists(m_testDatabaseName)) {
        QFile::remove(m_testDatabaseName);
    }
}

void
SqliteTest::defineTestDatabase()
{
    QSqlDatabase database = QSqlDatabase::addDatabase(m_driverName, TEST_CONNECTION_NAME);
    database.setDatabaseName(m_testDatabaseName);
}

QTEST_MAIN(SqliteTest)

#include "tst_SqliteTest.moc"
