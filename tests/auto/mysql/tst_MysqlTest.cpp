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
    void testCreateIndex();
};

MysqlTest::MysqlTest() : BasicTest(MYSQL_DRIVERNAME, MYSQLTEST_DATABASE_NAME
                            , &MysqlMigrator::buildContext, MYSQL_STRUCTURE_DATABASE
                            , MYSQL_HOSTNAME, MYSQL_HOSTPORT, MYSQL_USERNAME, MYSQL_PASSWORD)
{
}

void MysqlTest::testCreateIndex()
{
    base_testCreadeIndex(QString("SHOW INDEXES FROM testtable1"), 2);
}

QTEST_MAIN(MysqlTest)

#include "tst_MysqlTest.moc"
