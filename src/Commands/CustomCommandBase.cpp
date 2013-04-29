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
#include "CustomCommandBase.h"

namespace Commands {

CustomCommandBase::CustomCommandBase(const QString &commandName)
    : BaseCommand(CustomCommandBase::typeName())
    , m_customName(commandName)
{
    if(commandName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << CustomCommandBase::typeName() << "command with empty commandName!";
    }
}

const QString &CustomCommandBase::typeName()
{
    static QString sTypename("CustomCommand");
    return sTypename;
}

QString CustomCommandBase::customName() const
{
    return m_customName;
}

bool CustomCommandBase::down(const QSqlDatabase &database) const
{
    Q_UNUSED(database);
    return false;
}

} // namespace Commands
