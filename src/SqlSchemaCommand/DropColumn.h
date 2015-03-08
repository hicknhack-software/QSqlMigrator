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

#include "SqlSchema/Column.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

/// \brief value object representing the command to drop a column
class QSQLMIGRATOR_DLL_EXPORT DropColumn : public Command {
public:
    using TableName = QString;
    using ColumnName = QString;
    using Column = SqlSchema::Column;

public:
    /// Constructs the command to drop a column by name (but cannot restore it)
    explicit DropColumn(TableName, ColumnName);

    /// Constructs the command to drop a column with a column definition, allowing a restore
    explicit DropColumn(TableName, Column);

    static const CommandName &commandName() Q_DECL_NOEXCEPT;

    const TableName &tableName() const Q_DECL_NOEXCEPT;
    const ColumnName &columnName() const Q_DECL_NOEXCEPT;
    const Column &column() const Q_DECL_NOEXCEPT;

    CommandPtr reverse(const CommandPtr& selfPtr) const Q_DECL_OVERRIDE;

    void reportErrors(const Reporter &) const Q_DECL_OVERRIDE;

    String toString() const Q_DECL_OVERRIDE;

private:
    TableName m_tableName;
    Column m_column;
};

inline DropColumn::DropColumn(TableName tableName, ColumnName columnName)
    : Command(DropColumn::commandName())
    , m_tableName(std::move(tableName))
    , m_column(std::move(columnName), SqlSchema::ValueType::invalid())
{
}

inline DropColumn::DropColumn(TableName tableName, Column column)
    : Command(DropColumn::commandName())
    , m_tableName(std::move(tableName))
    , m_column(std::move(column))
{
}

inline const DropColumn::TableName &
DropColumn::tableName() const Q_DECL_NOEXCEPT
{
    return m_tableName;
}

inline const DropColumn::ColumnName &
DropColumn::columnName() const Q_DECL_NOEXCEPT
{
    return m_column.name();
}

inline const DropColumn::Column &
DropColumn::column() const Q_DECL_NOEXCEPT
{
    return m_column;
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
