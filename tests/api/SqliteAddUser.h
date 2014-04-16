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
#ifndef SQLITEADDUSER_H
#define SQLITEADDUSER_H

#include "Commands/CustomCommandBase.h"

#include <QSharedPointer>

class SqliteAddUser : public Commands::CustomCommandBase
{
public:
    SqliteAddUser(int id, const QString &userName, const QString &userMail, const QString &password);
    ~SqliteAddUser() QSQL_OVERRIDE_D {}

    bool up(const QSqlDatabase &database) const Q_DECL_OVERRIDE;

private:
    const int m_id;
    const QString m_userName;
    const QString m_userMail;
    const QString m_password;
};

#endif // SQLITEADDUSER_H
