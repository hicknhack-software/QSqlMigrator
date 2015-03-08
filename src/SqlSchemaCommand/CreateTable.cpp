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
#include "CreateTable.h"
#include "SqlSchemaCommand/DropTable.h"

#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

const CreateTable::CommandName &
CreateTable::commandName() Q_DECL_NOEXCEPT
{
    static CommandName typeName("CreateTable");
    return typeName;
}

CreateTable::CommandPtr
CreateTable::reverse(const CommandPtr&) const
{
    return QSharedPointer<DropTable>::create(m_table);
}

void
CreateTable::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return this->toString(); });
    Q_UNUSED(scope);

    m_table.reportErrors(reporter);
}

CreateTable::String
CreateTable::toString() const
{
    return QString("%1 named '%2'").arg(commandName()).arg(m_table.name());
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
