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
#include "M20131501_191807_CreateUsers.h"

QSQL_MIGRATION_IMPL(M20131501_191807_CreateUsers)

M20131501_191807_CreateUsers::M20131501_191807_CreateUsers()
{
    using namespace Structure;
    using namespace Commands;

    Table table("users");
    table << Column( "id", "int", Column::Primary )
          << Column( "name", "varchar(50)" )
          << Column( "email", "varchar(100)" )
          << Column( "password_salt", "varchar(64)" )
          << Column( "password_encrypted", "varchar(64)" );

    this->add(new CreateTable(table));

    Index emailIndex( "users_email", "users" );
    emailIndex.addColumn("email");

    this->add(new CreateIndex(emailIndex));
}
