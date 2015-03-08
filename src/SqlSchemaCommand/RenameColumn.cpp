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
#include "RenameColumn.h"

#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

const RenameColumn::CommandName &
RenameColumn::commandName() Q_DECL_NOEXCEPT
{
    static CommandName typeName("RenameColumn");
    return typeName;
}

RenameColumn::CommandPtr
RenameColumn::reverse(const CommandPtr&) const
{
    return QSharedPointer<RenameColumn>::create(m_tableName, m_newName, m_oldName);
}

void
RenameColumn::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return this->toString(); });
    Q_UNUSED(scope);

    if (tableName().isEmpty())
        reporter->error("name of table is empty");
    if (oldName().isEmpty())
        reporter->error("current name of column is empty");
    if (newName().isEmpty())
        reporter->error("new name of column is empty");
}

RenameColumn::String
RenameColumn::toString() const
{
    return QString("%1 to '%2' from %3 to %4").arg(commandName()).arg(m_tableName).arg(m_oldName).arg(m_newName);
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
