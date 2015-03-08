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
TARGET = SqlSchemaCommand
CONFIG += static
DEFINES += _BUILDING_QSQLMIGRATOR_DLL
DEFINES += LOG_PREFIX=\\\"[QSqlMigrator]\\\"

SOURCES += \
    AddColumn.cpp \
    AlterColumnType.cpp \
    Command.cpp \
    CreateIndex.cpp \
    CreateTable.cpp \
    DropColumn.cpp \
    DropIndex.cpp \
    DropTable.cpp \
    RenameColumn.cpp \
    RenameTable.cpp \
    CustomBase.cpp

INSTALL_HEADERS += \
    AlterColumnType.h \
    AddColumn.h \
    Command.h \
    CreateIndex.h \
    CreateTable.h \
    DropColumn.h \
    DropIndex.h \
    DropTable.h \
    RenameColumn.h \
    RenameTable.h

include(../../build/qmake/_lib.pri)

HEADERS += \
    CustomBase.h
