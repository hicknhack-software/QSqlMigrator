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
TEMPLATE = lib
CONFIG += dll

HEADERS += $$INSTALL_HEADERS

include(_common.pri)

DESTDIR = $$LIB_PATH

!isEmpty(INSTALL_PREFIX) {
    target.path = $$INSTALL_PREFIX
    INSTALLS += target
}
!isEmpty(INSTALL_HEADERS_PREFIX) {
    for(header, INSTALL_HEADERS) {
        path = $${dirname(header)}
        eval(headers_$${path}.files += $$header)
        eval(headers_$${path}.path = $$INSTALL_HEADERS_PREFIX/$$TARGET_INSTALL_HEADERS_PREFIX$$path)
        eval(win32:!isEmpty(headers_$${path}.extra): headers_$${path}.extra += &&)
        eval(win32:headers_$${path}.extra += $(COPY) \\\"$$shell_path($$_PRO_FILE_PWD_/$$header)\\\" \\\"$$shell_path($$INSTALL_HEADERS_PREFIX/$$TARGET_INSTALL_HEADERS_PREFIX$$path)\\\")
        eval(INSTALLS *= headers_$${path})
    }
}
