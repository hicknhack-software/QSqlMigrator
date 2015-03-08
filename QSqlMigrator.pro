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
TEMPLATE = subdirs

SUBDIRS += src
!CONFIG(NoPlugins): SUBDIRS += plugins
!CONFIG(NoTest): SUBDIRS += tests

plugins.depends = src
tests.depends = src plugins

OTHER_FILES += \
    README.md \
    CHANGES \
    LICENSE.GPL3 \
    LICENSE.LGPL \
    .clang-format \
    .travis.yml \
    appveyor.yml \
    build/qmake/_app.pri \
    build/qmake/_common.pri \
    build/qmake/_lib.pri \
    build/qmake/_plugin.pri \
    build/qmake/_test.pri \
