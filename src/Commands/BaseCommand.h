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
#ifndef COMMANDS_BASECOMMAND_H
#define COMMANDS_BASECOMMAND_H

#include "config.h"

#include <QString>
#include <QSharedPointer>

namespace Commands {

class BaseCommand;
typedef QSharedPointer<BaseCommand> CommandPtr;
typedef QSharedPointer<const BaseCommand> ConstCommandPtr;
typedef QList<CommandPtr> CommandPtrList;

class QSQLMIGRATOR_DLL_EXPORT BaseCommand
{
public:
     explicit BaseCommand(const QString &name);
     virtual ~BaseCommand() {}

    const QString &name() const;

private:
     QString m_name;
};

} // namespace Commands

#endif // COMMANDS_BASECOMMAND_H
