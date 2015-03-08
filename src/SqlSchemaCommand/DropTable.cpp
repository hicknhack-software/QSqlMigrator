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
#include "DropTable.h"

#include "SqlSchemaCommand/CreateTable.h"

#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

const DropTable::CommandName &
DropTable::commandName() Q_DECL_NOEXCEPT
{
    static CommandName typeName("DropTable");
    return typeName;
}

DropTable::CommandPtr
DropTable::reverse(const CommandPtr&) const
{
    if (!m_table.isValid())
        return {};
    return QSharedPointer<CreateTable>::create(m_table);
}

void
DropTable::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return this->toString(); });
    Q_UNUSED(scope);

    if (table().name().isEmpty())
        reporter->error("name of table is empty");
    else if (!table().isValid())
        reporter->warning("table is invalid (no reverse possible)");
}

DropTable::String
DropTable::toString() const
{
    return QString("%1 named '%2'").arg(commandName()).arg(m_table.name());
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
