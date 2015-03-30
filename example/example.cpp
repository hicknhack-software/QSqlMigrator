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
#include "MyAppMigrator.h"

#include "Config.h"

#include <QString>
#include <QSqlDatabase>
#include <QtTest>

class Example : public QObject
{
    Q_OBJECT

public:
    Example();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testMigrate();
    void testValidate();
};

Example::Example()
{}

void Example::initTestCase()
{
    if (!QString(EXAMPLE_DATABASE_FILENAME).isEmpty()) {
        QFile::remove(EXAMPLE_DATABASE_FILENAME);
    }

    ::qDebug() << "running test for example";
}

void Example::cleanupTestCase()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (database.isOpen()) database.close();

    if (!QString(EXAMPLE_DATABASE_FILENAME).isEmpty()) {
        if (QFile::exists(EXAMPLE_DATABASE_FILENAME)) {
            QFile::remove(EXAMPLE_DATABASE_FILENAME);
        }
    }
}

void Example::init()
{
    QSqlDatabase database = QSqlDatabase::database();;
    if(!QSqlDatabase::contains(/*default-connection*/)) {
        database = QSqlDatabase::addDatabase(EXAMPLE_DRIVERNAME);
        database.setDatabaseName(EXAMPLE_DATABASE);
    }
    auto success = database.open();
    QVERIFY2(success, "could not open database!");
}

void Example::cleanup()
{
    QSqlDatabase database = QSqlDatabase::database();;
    if (database.isOpen()) database.close();
}

void Example::testMigrate()
{
    QSqlDatabase database = QSqlDatabase::database();;
    auto success = MyAppMigrator::migrate(database);
    QVERIFY2(success, "MyAppMigrator::migrate");
}

void Example::testValidate()
{
    QSqlDatabase database = QSqlDatabase::database();;
    auto success = MyAppMigrator::validate(database);
    QVERIFY2(success, "MyAppMigrator::validate");
}

QTEST_MAIN(Example)

#include "example.moc"
