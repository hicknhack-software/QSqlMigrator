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
#include "CreateIndex.h"

#include "SqlSchemaCommand/DropIndex.h"

#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

const CreateIndex::CommandName &
CreateIndex::commandName() Q_DECL_NOEXCEPT
{
    static CommandName typeName("CreateIndex");
    return typeName;
}

CreateIndex::CommandPtr
CreateIndex::reverse(const CommandPtr&) const
{
    return QSharedPointer<DropIndex>::create(m_index);
}

void
CreateIndex::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return this->toString(); });
    Q_UNUSED(scope);

    m_index.reportErrors(reporter);
}

CreateIndex::String
CreateIndex::toString() const
{
    return QString("%1 named '%2' to table %3").arg(commandName()).arg(m_index.name()).arg(m_index.tableName());
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
