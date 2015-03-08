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

/// \brief value object representing the command to add a column to a table
class QSQLMIGRATOR_DLL_EXPORT AddColumn : public Command {
public:
    using Column = SqlSchema::Column;
    using TableName = QString;

public:
    explicit AddColumn(TableName, Column);

    static const CommandName &commandName() Q_DECL_NOEXCEPT;

    const TableName &tableName() const Q_DECL_NOEXCEPT;
    const Column &column() const Q_DECL_NOEXCEPT;

    CommandPtr reverse(const CommandPtr& selfPtr) const Q_DECL_OVERRIDE;

    void reportErrors(const Reporter &) const Q_DECL_OVERRIDE;

    String toString() const Q_DECL_OVERRIDE;

private:
    TableName m_tableName;
    Column m_column;
};

inline AddColumn::AddColumn(TableName tableName, Column column)
    : Command(AddColumn::commandName())
    , m_tableName(std::move(tableName))
    , m_column(std::move(column))
{
}

inline const AddColumn::TableName &
AddColumn::tableName() const Q_DECL_NOEXCEPT
{
    return m_tableName;
}

inline const AddColumn::Column &
AddColumn::column() const Q_DECL_NOEXCEPT
{
    return m_column;
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
