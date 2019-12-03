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
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/
#ifndef POSTGRESQLMIGRATOR_H
#define POSTGRESQLMIGRATOR_H

#include "MigrationExecution/MigrationExecutionContext.h"

#include <Qt>

#ifndef POSTGRESQLMIGRATOR_DLL_EXPORT
#   if defined(Q_OS_WIN) && !(defined(Q_CC_GNU) && defined(_BUILDING_STATIC_LIBS))
#       ifdef _BUILDING_POSTGRESQLMIGRATOR_DLL
#           define POSTGRESQLMIGRATOR_DLL_EXPORT __declspec(dllexport)
#       else
#           define POSTGRESQLMIGRATOR_DLL_EXPORT __declspec(dllimport)
#       endif
#   else
#       define POSTGRESQLMIGRATOR_DLL_EXPORT __attribute__ ((visibility("default")))
#   endif // Q_OS_WIN
#endif // QX_DLL_EXPORT_HELPER

namespace PostgresqlMigrator {

/*!
 * \brief Use this function to build your execution context for Postgresql migrations.
 *
 * See Sqlite for an usage example
 */
MigrationExecution::MigrationExecutionContextPtr POSTGRESQLMIGRATOR_DLL_EXPORT buildContext(MigrationExecution::MigrationExecutionContext::Builder &contextBuilder);

} // namespace PostgresqlMigrator

#endif // POSTGRESQLMIGRATOR_H
