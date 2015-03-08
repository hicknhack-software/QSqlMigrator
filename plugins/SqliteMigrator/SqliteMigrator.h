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
#pragma once

#include "SqlDatabaseAdapter/Adapter.h"
#include "SqlDatabaseSchemaAdapter/CommandExecutorRepository.h"

#include <Qt>

#ifndef SQLITEMIGRATOR_DLL_EXPORT
#   ifdef Q_OS_WIN
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

using Adapter = QSqlMigrator::SqlDatabaseAdapter::Adapter;
using CommandRepository = QSqlMigrator::SqlDatabaseSchemaAdapter::CommandExecutorRepository;

/*!
 * \brief Use this function to setup your execution context for sqlite migrations.
 *
 * Example:
 *
 * auto database = QSqlDatabase::addDatabase("QSQLITE");
 * database.setDatabaseName("sample_db.sqlite3");
 *
 * auto adapter = SqliteMigrator::createAdapter(database);
 *
 * auto migrator = QSqlMigrator({ migrations, tracker, SqliteMigrator::createCommandRepository(), adapter, logging });
 * // do something
 *
 */
SQLITEMIGRATOR_DLL_EXPORT Adapter
createAdapter(QSqlDatabase database);

SQLITEMIGRATOR_DLL_EXPORT CommandRepository
createCommandRepository();

} // namespace SqliteMigrator
