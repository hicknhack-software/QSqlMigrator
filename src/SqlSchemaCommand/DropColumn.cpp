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
#include "DropColumn.h"

#include "SqlSchemaCommand/AddColumn.h"

#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

const DropColumn::CommandName &
DropColumn::commandName() Q_DECL_NOEXCEPT
{
    static CommandName typeName("DropColumn");
    return typeName;
}

DropColumn::CommandPtr
DropColumn::reverse(const CommandPtr&) const
{
    if (!m_column.isValid())
        return {}; // no reverse command possible
    return QSharedPointer<AddColumn>::create(tableName(), column());
}

void
DropColumn::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return this->toString(); });
    Q_UNUSED(scope);

    if (tableName().isEmpty())
        reporter->error("name of table is empty");
    if (column().name().isEmpty())
        reporter->error("column name is invalid");
    else if (!column().isValid())
        reporter->warning("column is invalid (no reverse possible)");
}

DropColumn::String
DropColumn::toString() const
{
    return QString("%1 from '%2' named %3").arg(commandName()).arg(m_tableName).arg(m_column.name());
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
