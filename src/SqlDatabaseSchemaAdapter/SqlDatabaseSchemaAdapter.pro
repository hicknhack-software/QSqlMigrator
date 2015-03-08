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
TARGET = SqlDatabaseSchemaAdapter
CONFIG += static
DEFINES += _BUILDING_QSQLMIGRATOR_DLL
DEFINES += LOG_PREFIX=\\\"[QSqlMigrator]\\\"

INSTALL_HEADERS += \
    CommandExecutor.h \
    CommandExecutorContext.h \
    CommandExecutorRepository.h \
    CommandExecutor/StandardAddColumn.h \
    CommandExecutor/StandardAlterColumnType.h \
    CommandExecutor/StandardCreateIndex.h \
    CommandExecutor/StandardCreateTable.h \
    CommandExecutor/StandardDropColumn.h \
    CommandExecutor/StandardDropIndex.h \
    CommandExecutor/StandardDropTable.h \
    CommandExecutor/StandardRenameColumn.h \
    CommandExecutor/StandardRenameTable.h

SOURCES += \
    CommandExecutor.cpp \
    CommandExecutorRepository.cpp \
    CommandExecutor/StandardAddColumn.cpp \
    CommandExecutor/StandardAlterColumnType.cpp \
    CommandExecutor/StandardCreateIndex.cpp \
    CommandExecutor/StandardCreateTable.cpp \
    CommandExecutor/StandardDropColumn.cpp \
    CommandExecutor/StandardDropIndex.cpp \
    CommandExecutor/StandardDropTable.cpp \
    CommandExecutor/StandardRenameColumn.cpp \
    CommandExecutor/StandardRenameTable.cpp \
    CommandExecutor/StandardCustom.cpp

include(../../build/qmake/_lib.pri)

HEADERS += \
    CommandExecutor/StandardCustom.h
