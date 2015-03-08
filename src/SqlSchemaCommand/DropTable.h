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

#include "SqlSchema/Table.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

/// \brief value object representing the command to drop a table
class QSQLMIGRATOR_DLL_EXPORT DropTable : public Command {
public:
    using Table = SqlSchema::Table;
    using TableName = Table::Name;

public:
    explicit DropTable(TableName);
    explicit DropTable(Table);

    static const CommandName &commandName() Q_DECL_NOEXCEPT;

    const Table &table() const Q_DECL_NOEXCEPT;
    const TableName &tableName() const Q_DECL_NOEXCEPT;

    CommandPtr reverse(const CommandPtr& selfPtr) const Q_DECL_OVERRIDE;

    void reportErrors(const Reporter &) const Q_DECL_OVERRIDE;

    String toString() const Q_DECL_OVERRIDE;

private:
    Table m_table;
};

inline DropTable::DropTable(TableName tableName)
    : Command(DropTable::commandName())
    , m_table(std::move(tableName), {})
{
}

inline DropTable::DropTable(Table table)
    : Command(DropTable::commandName())
    , m_table(std::move(table))
{
}

inline const DropTable::Table &
DropTable::table() const Q_DECL_NOEXCEPT
{
    return m_table;
}

inline const DropTable::TableName &
DropTable::tableName() const Q_DECL_NOEXCEPT
{
    return m_table.name();
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
