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

    Table::Builder table("users");
    table << Column( "id", Type::Integer, Column::Primary )
          << Column( "name", Type(Type::VarChar, 50) )
          << Column( "email", Type(Type::VarChar, 100) )
          << Column( "password_salt", Type(Type::VarChar, 64) )
          << Column( "password_encrypted", Type(Type::VarChar, 64) );

    this->add(new CreateTable(table));

    Index::Builder emailIndex( "users_email", "users" );
    emailIndex << Index::Column("email");

    this->add(new CreateIndex(emailIndex));
}
