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

#include "SqlSchema/Index.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

/// \brief value object representing the command to drop an index
class QSQLMIGRATOR_DLL_EXPORT DropIndex : public Command
{
public:
    using IndexName = QString;
    using Index = SqlSchema::Index;

public:
    explicit DropIndex(IndexName);
    explicit DropIndex(Index);

    static const CommandName &commandName() Q_DECL_NOEXCEPT;

    const Index &index() const Q_DECL_NOEXCEPT;
    const IndexName &name() const Q_DECL_NOEXCEPT;

    CommandPtr reverse(const CommandPtr& selfPtr) const Q_DECL_OVERRIDE;

    void reportErrors(const Reporter &) const Q_DECL_OVERRIDE;

    String toString() const Q_DECL_OVERRIDE;

private:
    Index m_index;
};

inline DropIndex::DropIndex(IndexName indexName)
    : Command(DropIndex::commandName())
    , m_index(std::move(indexName), {}, {})
{
}

inline DropIndex::DropIndex(Index index)
    : Command(DropIndex::commandName())
    , m_index(std::move(index))
{
}

inline const DropIndex::Index &
DropIndex::index() const Q_DECL_NOEXCEPT
{
    return m_index;
}

inline const DropIndex::IndexName &
DropIndex::name() const Q_DECL_NOEXCEPT
{
    return m_index.name();
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
