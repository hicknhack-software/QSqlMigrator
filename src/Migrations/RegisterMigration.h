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
#ifndef MIGRATIONS_REGISTERMIGRATION_H
#define MIGRATIONS_REGISTERMIGRATION_H

#include "Migrations/MigrationRepository.h"

#define QSQL_MIGRATION(MigrationClass) \
     extern Migrations::RegisterMigration<MigrationClass> RegisterMigration_##MigrationClass;

#define QSQL_MIGRATION_IMPL(MigrationClass) \
     Migrations::RegisterMigration<MigrationClass> RegisterMigration_##MigrationClass(#MigrationClass);

namespace Migrations {

template< class MigrationClass >
class RegisterMigration
{
public:
     RegisterMigration(const QString &fullName)
     {
          MigrationRepository::storeMigration(fullName, &m_migration);
     }

private:
     const MigrationClass m_migration;
};

} // namespace Migrations

#endif // MIGRATIONS_REGISTERMIGRATION_H
