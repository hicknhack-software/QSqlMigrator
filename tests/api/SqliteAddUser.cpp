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

SqliteAddUser::SqliteAddUser(int id, const QString &userName, const QString &userMail, const QString &password)
    : CustomCommandBase("SqliteAddUser")
    , m_id(id)
    , m_userName(userName)
    , m_userMail(userMail)
    , m_password(password)
{
}

bool SqliteAddUser::up(const QSqlDatabase &database) const
{
    QString insertQuery("INSERT INTO users (id, name, email, password_encrypted) VALUES ('%1', '%2', '%3', '%4')");
    insertQuery = insertQuery.arg(m_id).arg(m_userName).arg(m_userMail).arg(m_password);

    ::qDebug() << "complete query-string looks like:";
    ::qDebug() << insertQuery;
    QSqlQuery query = database.exec(insertQuery);
    QSqlError error = query.lastError();
    if (error.isValid()) {
         ::qDebug() << Q_FUNC_INFO << error.text();
         return false;
    }
    return true;
}
