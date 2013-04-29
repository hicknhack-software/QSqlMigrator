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
#ifndef MIGRATIONS_MIGRATIONREPOSITORY_H
#define MIGRATIONS_MIGRATIONREPOSITORY_H

#include "Migrations/Migration.h"

#include <QMap>
#include <QStringList>

namespace Migrations {

class QSQLMIGRATOR_DLL_EXPORT MigrationRepository
{
public:
    typedef QMap<QString, const Migration*> NameMigrationMap;

    static const NameMigrationMap &migrations();

    static void storeMigration(const QString &migrationName, const Migration *migration);

private:
    MigrationRepository() {}
    MigrationRepository(const MigrationRepository &);
    MigrationRepository &operator=(const MigrationRepository &);
    ~MigrationRepository() {}

    static MigrationRepository &getInstance();

private:
    NameMigrationMap m_migrationMap;
};

} // namespace Migrations

#endif // MIGRATIONS_MIGRATIONREPOSITORY_H
