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
TARGET = SqliteMigrator
DEFINES += _BUILDING_SQLITEMIGRATOR_DLL

SOURCES += \
    DatabaseLock.cpp \
    CommandExecution/SqliteDropColumnService.cpp \
    CommandExecution/SqliteAlterColumnTypeService.cpp \
    CommandExecution/SqliteAlterColumnService.cpp \
    CommandExecution/SqliteRenameColumnService.cpp \
    MigrationTracker/SqliteMigrationTableService.cpp \
    SqliteMigrator.cpp \
    Helper/SqliteSqlStructureService.cpp

INSTALL_HEADERS += \
    SqliteMigrator.h \
    DatabaseLock.h

HEADERS += \
    CommandExecution/SqliteDropColumnService.h \
    CommandExecution/SqliteAlterColumnTypeService.h \
    CommandExecution/SqliteAlterColumnService.h \
    CommandExecution/SqliteRenameColumnService.h \
    MigrationTracker/SqliteMigrationTableService.h \
    Helper/SqliteSqlStructureService.h

include(../../build/qmake/_plugin.pri)
