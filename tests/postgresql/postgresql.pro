# Copyright (C) 2014, HicknHack Software
# All rights reserved.
# Contact: http://www.hicknhack-software.com/contact
#
# This file is part of the QSqlMigrator
#
# GNU Lesser General Public License Usage
# This file may be used under the terms of the GNU Lesser
# General Public License version 2.1 as published by the Free Software
# Foundation and appearing in the file LICENSE.LGPL included in the
# packaging of this file.  Please review the following information to
# ensure the GNU Lesser General Public License version 2.1 requirements
# will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL3 included in the
# packaging of this file.  Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl.html.
TARGET = TestPostgresql
PROJECT_ROOT = $$PWD/../..

!exists(PostgresqlConfig.h) {
  system($$QMAKE_COPY PostgresqlConfig.h.example PostgresqlConfig.h)
}

SOURCES += tst_PostgresqlTest.cpp \
    ../BasicTest/BasicTest.cpp

HEADERS += \
    ../BasicTest/BasicTest.h \
    PostgresqlConfig.h \
    PostgresqlConfig.h.example

OTHER_FILES += \
    PostgresqlDriver.txt \
    PostgresqlConfig.h.appveyor \
    PostgresqlConfig.h.travis

include($$PROJECT_ROOT/build/qmake/_test.pri)

# PostgresqlMigrator {
LIBS += -lPostgresqlMigrator

DEPENDPATH += $$LIB_PATH

win32: PRE_TARGETDEPS += $$LIB_PATH/PostgresqlMigrator.dll
else: PRE_TARGETDEPS += $$LIB_PATH/libPostgresqlMigrator.so
# }
