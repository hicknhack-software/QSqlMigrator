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
#include "AddColumn.h"
#include "SqlSchemaCommand/DropColumn.h"

#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

const AddColumn::CommandName &
AddColumn::commandName() Q_DECL_NOEXCEPT
{
    static CommandName typeName("AddColumn");
    return typeName;
}

AddColumn::CommandPtr
AddColumn::reverse(const CommandPtr&) const
{
    return QSharedPointer<DropColumn>::create(tableName(), column());
}

void
AddColumn::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return this->toString(); });
    Q_UNUSED(scope);

    if (m_tableName.isEmpty())
        reporter->error("name of table is empty");

    m_column.reportErrors(reporter);
}

AddColumn::String
AddColumn::toString() const
{
    return QString("%1 to '%2' named %3").arg(commandName()).arg(m_tableName).arg(m_column.name());
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
