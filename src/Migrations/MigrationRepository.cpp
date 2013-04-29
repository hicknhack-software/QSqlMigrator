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
#include "MigrationRepository.h"

#include <QDebug>

namespace Migrations {

MigrationRepository &MigrationRepository::getInstance()
{
    static MigrationRepository *instance = 0;
    if( ! instance ) {
        instance = new MigrationRepository();
    }
    return (*instance);
}

const MigrationRepository::NameMigrationMap &MigrationRepository::migrations()
{
    return getInstance().m_migrationMap;
}

void MigrationRepository::storeMigration(const QString &migrationName, const Migration * migration)
{
    if(migrationName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << "migrationName is empty. Can't store migration.";
        return;
    }

    if(!migration) {
        ::qWarning() << LOG_PREFIX << "migration is 0. Can't store migration.";
        return;
    }

    if(MigrationRepository::getInstance().m_migrationMap.contains(migrationName)) {
        ::qWarning() << LOG_PREFIX << "migration" << migrationName << "already stored!";
        return;
    }

    MigrationRepository::getInstance().m_migrationMap[migrationName] = migration;
}

MigrationRepository &MigrationRepository::operator =(const MigrationRepository &)
{
    return (*this);
}

} // namespace Migrations
