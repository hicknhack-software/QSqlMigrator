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
TARGET = SqlMemorySchema
CONFIG += static
DEFINES += _BUILDING_QSQLMIGRATOR_DLL
DEFINES += LOG_PREFIX=\\\"[QSqlMigrator]\\\"

SOURCES += \
    SchemaCommandExecutorRepository.cpp \
    CommandExecutor/SchemaAddColumn.cpp \
    CommandExecutor/SchemaAlterColumnType.cpp \
    CommandExecutor/SchemaCreateIndex.cpp \
    CommandExecutor/SchemaCreateTable.cpp \
    CommandExecutor/SchemaDropColumn.cpp \
    CommandExecutor/SchemaDropIndex.cpp \
    CommandExecutor/SchemaDropTable.cpp \
    CommandExecutor/SchemaRenameColumn.cpp \
    CommandExecutor/SchemaRenameTable.cpp \
    SchemaMigrator.cpp

INSTALL_HEADERS += \

HEADERS += \
    SchemaCommandExecutor.h \
    SchemaCommandExecutorRepository.h \
    CommandExecutor/SchemaAddColumn.h \
    CommandExecutor/SchemaAlterColumnType.h \
    CommandExecutor/SchemaCreateIndex.h \
    CommandExecutor/SchemaCreateTable.h \
    CommandExecutor/SchemaDropColumn.h \
    CommandExecutor/SchemaDropIndex.h \
    CommandExecutor/SchemaDropTable.h \
    CommandExecutor/SchemaRenameColumn.h \
    CommandExecutor/SchemaRenameTable.h \
    SchemaMigrator.h

include(../../build/qmake/_lib.pri)
