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
#include "AlterColumnType.h"

#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

const AlterColumnType::CommandName &
AlterColumnType::commandName() Q_DECL_NOEXCEPT
{
    static CommandName typeName("AlterColumnType");
    return typeName;
}

AlterColumnType::CommandPtr
AlterColumnType::reverse(const CommandPtr&) const
{
    if (!m_oldType.isValid())
        return {};
    return QSharedPointer<AlterColumnType>::create(tableName(), columnName(), oldType(), newType());
}

void
AlterColumnType::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return this->toString(); });
    Q_UNUSED(scope);

    if (m_columnName.isEmpty())
        reporter->error("name of column is empty");
    if (m_tableName.isEmpty())
        reporter->error("name of table is empty");
    if (!m_newType.isValid())
        reporter->error("new type is invalid");
    if (!m_oldType.isValid())
        reporter->warning("old type is invalid (no reverse possible)");
}

AlterColumnType::String
AlterColumnType::toString() const
{
    return QString("%1 to '%2' named %3").arg(commandName()).arg(m_tableName).arg(m_columnName);
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
