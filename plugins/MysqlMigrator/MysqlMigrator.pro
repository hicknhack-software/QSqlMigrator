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
TARGET = MysqlMigrator
DEFINES += _BUILDING_MYSQLMIGRATOR_DLL

SOURCES += \
     MysqlMigrator.cpp \
     Helper/MysqlStructureService.cpp \
     Helper/MysqlColumnService.cpp \
     Helper/MysqlTypeMapperService.cpp \
     CommandExecution/MysqlAlterColumnTypeService.cpp \
     CommandExecution/MysqlRenameColumnService.cpp

INSTALL_HEADERS += \
     MysqlMigrator.h

HEADERS += \
     Helper/MysqlStructureService.h \
     Helper/MysqlColumnService.h \
     Helper/MysqlTypeMapperService.h \
     CommandExecution/MysqlAlterColumnTypeService.h \
     CommandExecution/MysqlRenameColumnService.h

include(../../build/qmake/_plugin.pri)
