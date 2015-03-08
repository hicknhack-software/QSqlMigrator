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

#include "SqlMigration/Migration.h"

#include <QList>

namespace QSqlMigrator {
namespace SqlMigration {

/// \brief constant value object representing a set of migrations
class QSQLMIGRATOR_DLL_EXPORT MigrationRepository {
public:
    using Migration = SqlMigration::Migration;
    using MigrationName = Migration::Name;
    using Migrations = QList<Migration>;
    using MigrationNames = QList<MigrationName>;

public:
    MigrationRepository(Migrations);

    /// \return repository with the Migrations from the Builder classes
    /// \example
    /// const auto rep = Migration::Repository::fromBuilders<M20141224_235627_CreateUsers>();
    template <typename... MigrationBuilders>
    static MigrationRepository fromBuilders();

    const Migrations &migrations() const;

    MigrationNames migrationNames() const;

    const Migration *findMigration(const MigrationName &) const;

private:
    Migrations m_migrations;
};

inline MigrationRepository::MigrationRepository(Migrations migrations)
    : m_migrations(std::move(migrations))
{
}

#ifdef _MSC_BUILD
template <typename... MigrationBuilders>
struct BuildMigrations;

template <typename T>
struct BuildMigrations<T> {
    static void
    build(QList<Migration> &list)
    {
        list.push_back(T());
    }
};

template <typename T, typename... TS>
struct BuildMigrations<T, TS...> {
    static void
    build(QList<Migration> &list)
    {
        BuildMigrations<TS...>::build(list);
        list.push_back(T());
    }
};
#endif

template <typename... MigrationBuilders>
MigrationRepository
MigrationRepository::fromBuilders()
{
#ifdef _MSC_BUILD
    QList<Migration> migrations;
    BuildMigrations<MigrationBuilders...>::build(migrations);
    return {migrations};
#else
    return {{(Migration)MigrationBuilders()...}};
#endif
}

inline const MigrationRepository::Migrations &
MigrationRepository::migrations() const
{
    return m_migrations;
}

inline MigrationRepository::MigrationNames
MigrationRepository::migrationNames() const
{
    MigrationNames names;
    for (const auto &migration : m_migrations)
        names << migration.name();
    return names;
}

inline const MigrationRepository::Migration *
MigrationRepository::findMigration(const MigrationRepository::MigrationName &migrationName) const
{
    for (const auto &migration : m_migrations)
        if (migration.name() == migrationName)
            return &migration;
    return Q_NULLPTR;
}

} // namespace SqlMigration
} // namespace QSqlMigrator
