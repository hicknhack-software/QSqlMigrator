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
#include "Commands/CreateTable.h"

#include "Commands/DropTable.h"

namespace Commands {

CreateTable::CreateTable(const Structure::Table &table)
    : BaseCommand(CreateTable::typeName())
    , m_table(table)
{
}

const QString &CreateTable::typeName()
{
    static QString typeName("CreateTable");
    return typeName;
}

CommandPtr CreateTable::reverse() const
{
    return CommandPtr(new DropTable(table()));
}

const Structure::Table &CreateTable::table() const
{
    return m_table;
}

} // namespace Commands
