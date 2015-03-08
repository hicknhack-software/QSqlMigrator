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
#include "SqliteAddUser.h"

SqliteAddUser::SqliteAddUser(UserId userId, UserName userName, UserMail userMail, Password password)
    : CustomBase("SqliteAddUser")
    , m_userId(userId)
    , m_userName(userName)
    , m_userMail(userMail)
    , m_password(password)
{
}

bool
SqliteAddUser::up(const QSqlDatabase &database) const
{
    static const auto insertQuery =
        QString("INSERT INTO users (id, name, email, password_encrypted) "
                "VALUES (:id, :name, :email, :pw)");

    auto query = QSqlQuery(database);
    query.prepare(insertQuery);
    query.bindValue(":id", m_userId);
    query.bindValue(":name", m_userName);
    query.bindValue(":email", m_userMail);
    query.bindValue(":pw", m_password);
    query.exec();

    QSqlError error = query.lastError();
    if (error.isValid()) {
        ::qDebug() << Q_FUNC_INFO << error.text();
        return false;
    }
    return true;
}
