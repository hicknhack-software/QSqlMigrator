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

#include "Commands/CreateIndex.h"

#include <QDebug>

namespace Commands {

DropIndex::DropIndex(const QString &name)
    : BaseCommand(DropIndex::typeName())
    , m_index(Structure::Index(name, Structure::Index::ColumnList()))
{
    if (name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << DropIndex::typeName() << "command with empty name!";
    }
}

DropIndex::DropIndex(const Structure::Index &index)
    : BaseCommand(DropIndex::typeName())
    , m_index(index)
{
    if (!m_index.isValid()) {
        ::qWarning() << LOG_PREFIX << DropIndex::typeName() << "command with invalid index!";
    }
}

const QString &DropIndex::typeName()
{
    static QString typeName("DropIndex");
    return typeName;
}

CommandPtr DropIndex::reverse() const
{
    if (!m_index.isValid())
        return CommandPtr();
    return CommandPtr(new CreateIndex(index()));
}

} // namespace Commands
