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
#ifndef SQLITEMIGRATOR_H
#define SQLITEMIGRATOR_H

#include "MigrationExecution/MigrationExecutionContext.h"

#include <Qt>

#ifndef SQLITEMIGRATOR_DLL_EXPORT
#   if defined(Q_OS_WIN) && !(defined(Q_CC_GNU) && defined(_BUILDING_STATIC_LIBS))
#       ifdef _BUILDING_SQLITEMIGRATOR_DLL
#           define SQLITEMIGRATOR_DLL_EXPORT __declspec(dllexport)
#       else
#           define SQLITEMIGRATOR_DLL_EXPORT __declspec(dllimport)
#       endif
#   else
#       define SQLITEMIGRATOR_DLL_EXPORT __attribute__ ((visibility("default")))
#   endif // Q_OS_WIN
#endif // SQLITEMIGRATOR_DLL_EXPORT

namespace SqliteMigrator {

/*!
 * \brief Use this function to setup your execution context for sqlite migrations.
 *
 * Example:
 *
 * MigrationExecutionContext::Builder builder(...);
 *
 * QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
 * db.setDatabaseName("sample_db.sqlite3");
 * builder.setDatabase(db);
 *
 * MigrationExecutionContextPtr context( SqliteMigrator::buildContext(builder) );
 * if (context) {
 *      // do something
 * }
 *
 */
MigrationExecution::MigrationExecutionContextPtr SQLITEMIGRATOR_DLL_EXPORT buildContext(MigrationExecution::MigrationExecutionContext::Builder &contextBuilder);

} // namespace SqliteMigrator

#endif // SQLITEMIGRATOR_H
