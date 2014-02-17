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
#ifndef MIGRATIONEXECUTION_MIGRATIONEXECUTIONCONFIG_H
#define MIGRATIONEXECUTION_MIGRATIONEXECUTIONCONFIG_H

#include "config.h"

#include <QString>

namespace MigrationExecution {

struct QSQLMIGRATOR_DLL_EXPORT MigrationExecutionConfig
{
     MigrationExecutionConfig(const QString &migrationVersionTableName = "qt_migrator_version_table",
                              const QString &temporaryTablePrefix = "temp_",
                              const QString &migrationTablePrefix = "");

     QString migrationVersionTableName;
     QString migrationTablePrefix;
     QString temporaryTablePrefix;
     bool createDatabase;
};

} // namespace MigrationExecution

#endif // MIGRATIONEXECUTION_MIGRATIONEXECUTIONCONFIG_H
