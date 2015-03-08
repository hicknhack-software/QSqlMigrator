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

#include "SqlSchemaCommand/Command.h"

#include "config.h"

#include <QString>
#include <QSharedPointer>
#include <utility>

namespace QSqlMigrator {
namespace SqlMigration {

/// \brief value class representing a sql schema migration
/// \note use a descriptive, sortable name (hint: start with the date)
/// \note a migration should transform the database between two valid states
/// \note use custom commands for data migrations (add your own schema commands if necessary)
/// \note build each migration into a seperate file in a way you never have to touch it again
/// \example using inheritance from MigrationBuilder
/// class M20141224_235627_CreateUsers : public SqlMigration::MigrationBuilder {
///     M20141224_235627_CreateUsers()
///         : MigrationBuilder("M20141224_235627_CreateUsers")
///     {
///         auto table = SqlSchema::TableBuilder("users");
///         table << SqlSchema::Column("id", SqlSchema::Type::Integer, SqlSchema::Column::PrimaryKey);
///         // ... more columns
///         addCommand<CreateTable>(table);
///     }
/// };
class QSQLMIGRATOR_DLL_EXPORT Migration {
public:
    using Name = QString;
    using Command = QSharedPointer<const SqlSchemaCommand::Command>;
    using CommandList = QList<Command>;

public:
    explicit Migration(Name, CommandList);

    const Name &name() const;
    const CommandList &commandList() const;

private:
    Name m_name;
    CommandList m_commandList;
};

/// \brief builder class for migrations
class QSQLMIGRATOR_DLL_EXPORT MigrationBuilder {
public:
    using Name = QString;
    using Command = QSharedPointer<const SqlSchemaCommand::Command>;
    using CommandList = QList<Command>;

public:
    explicit MigrationBuilder(Name, CommandList = {});

    operator Migration() const;

    MigrationBuilder &operator<<(Command);

    /// constructs and appends a command to the migration
    template <typename CommandType, typename... Args>
    MigrationBuilder &addCommand(Args &&...);

private:
    Name m_name;
    CommandList m_commandList;
};

inline Migration::Migration(Name name, CommandList commandList)
    : m_name(std::move(name))
    , m_commandList(std::move(commandList))
{
}

inline const Migration::Name &
Migration::name() const
{
    return m_name;
}

inline const Migration::CommandList &
Migration::commandList() const
{
    return m_commandList;
}

inline MigrationBuilder::MigrationBuilder(Name name, CommandList commandList)
    : m_name(std::move(name))
    , m_commandList(std::move(commandList))
{
}

inline MigrationBuilder::operator Migration() const
{
    return Migration{m_name, m_commandList};
}

inline MigrationBuilder &MigrationBuilder::operator<<(Command command)
{
    m_commandList.append(std::move(command));
    return *this;
}

template <typename CommandType, typename... Args>
inline MigrationBuilder &
MigrationBuilder::addCommand(Args &&... args)
{
    m_commandList.push_back(QSharedPointer<CommandType>::create(std::forward<Args>(args)...));
    return *this;
}

} // namespace SqlMigration
} // namespace QSqlMigrator
