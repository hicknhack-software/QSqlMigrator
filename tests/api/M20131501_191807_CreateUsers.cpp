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

#include "SqlSchema/Table.h"
#include "SqlSchema/Index.h"
#include "SqlSchemaCommand/CreateTable.h"
#include "SqlSchemaCommand/CreateIndex.h"

M20131501_191807_CreateUsers::M20131501_191807_CreateUsers()
    : MigrationBuilder("M20131501_191807_CreateUsers")
{
    using namespace QSqlMigrator::SqlSchema;
    using namespace QSqlMigrator::SqlSchemaCommand;

    auto table = TableBuilder("users");
    table << Column( "id", ValueType(Type::Integer), Column::Primary )
          << Column( "name", ValueType(Type::VarChar, 50) )
          << Column( "email", ValueType(Type::VarChar, 100) )
          << Column( "password_salt", ValueType(Type::VarChar, 64) )
          << Column( "password_encrypted", ValueType(Type::VarChar, 64) );

    addCommand<CreateTable>(table);

    auto emailIndex = IndexBuilder( "users_email", "users" );
    emailIndex << IndexColumn("email");

    addCommand<CreateIndex>(emailIndex);
}
