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

#include <QString>

namespace QSqlMigrator {
namespace SqlSchemaCommand {

/// \brief value object representing the command to rename a column of a table
class QSQLMIGRATOR_DLL_EXPORT RenameColumn : public Command {
public:
    using TableName = QString;
    using ColumnName = QString;

public:
    explicit RenameColumn(TableName, ColumnName oldName, ColumnName newName);

    static const QString &commandName() Q_DECL_NOEXCEPT;

    const TableName &tableName() const Q_DECL_NOEXCEPT;
    const ColumnName &oldName() const Q_DECL_NOEXCEPT;
    const ColumnName &newName() const Q_DECL_NOEXCEPT;

    CommandPtr reverse(const CommandPtr& selfPtr) const Q_DECL_OVERRIDE;

    void reportErrors(const Reporter &) const Q_DECL_OVERRIDE;

    String toString() const Q_DECL_OVERRIDE;

private:
    TableName m_tableName;
    QString m_oldName;
    QString m_newName;
};

inline RenameColumn::RenameColumn(TableName tableName, ColumnName oldName, ColumnName newName)
    : Command(RenameColumn::commandName())
    , m_tableName(std::move(tableName))
    , m_oldName(std::move(oldName))
    , m_newName(std::move(newName))
{
}

inline const RenameColumn::TableName &
RenameColumn::tableName() const Q_DECL_NOEXCEPT
{
    return m_tableName;
}

inline const RenameColumn::ColumnName &
RenameColumn::oldName() const Q_DECL_NOEXCEPT
{
    return m_oldName;
}

inline const RenameColumn::ColumnName &
RenameColumn::newName() const Q_DECL_NOEXCEPT
{
    return m_newName;
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
