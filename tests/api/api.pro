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
TARGET = TestApi

!exists(ApiConfig.h) {
    system($$QMAKE_COPY ApiConfig.h.example ApiConfig.h)
}

SOURCES += tst_ApiTest.cpp \
    M20132201_180943_CreateCars.cpp \
    M20132201_175827_CreateAddresses.cpp \
    M20131501_191807_CreateUsers.cpp \
    M20133001_164323_AddUsers.cpp \
    SqliteAddUser.cpp

HEADERS += \
    M20132201_180943_CreateCars.h \
    M20132201_175827_CreateAddresses.h \
    M20131501_191807_CreateUsers.h \
    M20133001_164323_AddUsers.h \
    SqliteAddUser.h \
    ApiConfig.h \
    ApiConfig.h.example

include(../../build/qmake/_test.pri)

# SqliteMigrator {
LIBS += -lSqliteMigrator

DEPENDPATH += $$LIB_PATH

win32: PRE_TARGETDEPS += $$LIB_PATH/SqliteMigrator.dll
macx: PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigrator.dylib
else: PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigrator.so
# }
