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
#include "DropIndex.h"

#include "SqlSchemaCommand/CreateIndex.h"

#include "LoggingTrace/Logging.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

const DropIndex::CommandName &
DropIndex::commandName() Q_DECL_NOEXCEPT
{
    static QString typeName("DropIndex");
    return typeName;
}

DropIndex::CommandPtr
DropIndex::reverse(const CommandPtr&) const
{
    if (!m_index.isValid())
        return {};
    return QSharedPointer<CreateIndex>::create(m_index);
}

void
DropIndex::reportErrors(const Reporter &reporter) const
{
    auto scope = reporter->scope([&]() { return this->toString(); });
    Q_UNUSED(scope);

    if (index().name().isEmpty())
        reporter->error("name of index is empty");
    else if (!index().isValid())
        reporter->warning("index is invalid (no reverse possible)");
}

DropIndex::String
DropIndex::toString() const
{
    return QString("%1 named '%2'").arg(commandName()).arg(m_index.name());
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
