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

#include "SqlSchema/ValueType.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

/// \brief value object representing the command to alter the type of a column in a table
class QSQLMIGRATOR_DLL_EXPORT AlterColumnType : public Command {
public:
    using TableName = QString;
    using ColumnName = QString;
    using ColumnType = SqlSchema::ValueType;

public:
    explicit AlterColumnType(TableName, ColumnName, ColumnType newType, ColumnType oldType);
    explicit AlterColumnType(TableName, ColumnName, ColumnType newType);

    static const CommandName &commandName() Q_DECL_NOEXCEPT;

    const TableName &tableName() const Q_DECL_NOEXCEPT;
    const ColumnName &columnName() const Q_DECL_NOEXCEPT;
    const ColumnType &newType() const Q_DECL_NOEXCEPT;
    const ColumnType &oldType() const Q_DECL_NOEXCEPT;

    CommandPtr reverse(const CommandPtr& selfPtr) const Q_DECL_OVERRIDE;

    void reportErrors(const Reporter &) const Q_DECL_OVERRIDE;

    String toString() const Q_DECL_OVERRIDE;

private:
    const TableName m_tableName;
    const ColumnName m_columnName;
    const ColumnType m_newType;
    const ColumnType m_oldType;
};

inline AlterColumnType::AlterColumnType(TableName tableName,
                                        ColumnName columnName,
                                        ColumnType newType,
                                        ColumnType oldType)
    : Command(AlterColumnType::commandName())
    , m_tableName(std::move(tableName))
    , m_columnName(std::move(columnName))
    , m_newType(std::move(newType))
    , m_oldType(std::move(oldType))
{
}

inline AlterColumnType::AlterColumnType(TableName tableName, ColumnName columnName, ColumnType newType)
    : Command(AlterColumnType::commandName())
    , m_tableName(std::move(tableName))
    , m_columnName(std::move(columnName))
    , m_newType(std::move(newType))
    , m_oldType(ColumnType::invalid())
{
}

inline const AlterColumnType::TableName &
AlterColumnType::tableName() const Q_DECL_NOEXCEPT
{
    return m_tableName;
}

inline const AlterColumnType::ColumnName &
AlterColumnType::columnName() const Q_DECL_NOEXCEPT
{
    return m_columnName;
}

inline const AlterColumnType::ColumnType &
AlterColumnType::newType() const Q_DECL_NOEXCEPT
{
    return m_newType;
}

inline const AlterColumnType::ColumnType &
AlterColumnType::oldType() const Q_DECL_NOEXCEPT
{
    return m_oldType;
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
