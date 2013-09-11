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

#include <QDebug>

namespace Commands {

DropIndex::DropIndex(const QString &name)
    : BaseCommand(DropIndex::typeName())
    , m_hasIndex(false)
    , m_index(Structure::Index("", ""))
    , m_name(name)
{
    if(name.isEmpty()) {
        ::qWarning() << LOG_PREFIX << DropIndex::typeName() << "command with empty name!";
    }
}

DropIndex::DropIndex(const Structure::Index &index)
    : BaseCommand(DropIndex::typeName())
    , m_hasIndex(true)
    , m_index(index)
{
}

bool DropIndex::hasIndex() const
{
    return m_hasIndex;
}

const Structure::Index &DropIndex::index() const
{
    return m_index;
}

const QString &DropIndex::typeName()
{
    static QString sTypename("DropIndex");
    return sTypename;
}

const QString &DropIndex::name() const
{
    if (this->hasIndex()) {
        return m_index.name();
    } else {
        return m_name;
    }
}

} // namespace Commands
