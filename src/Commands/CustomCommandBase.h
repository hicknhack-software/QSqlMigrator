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
#ifndef COMMANDS_CUSTOMCOMMAND_H
#define COMMANDS_CUSTOMCOMMAND_H

#include "Commands/BaseCommand.h"

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

namespace Commands {

class QSQLMIGRATOR_DLL_EXPORT CustomCommandBase : public BaseCommand
{
public:
    explicit CustomCommandBase(const QString &commandName);

    virtual QString customName() const;
    static const QString &typeName();

    virtual bool up(const QSqlDatabase &database) const = 0;
    virtual bool down(const QSqlDatabase &database) const;

private:
    QString m_customName;
};

} // namespace Commands

#endif // COMMANDS_CUSTOMCOMMAND_H
