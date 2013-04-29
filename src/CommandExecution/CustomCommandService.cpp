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
#include "CustomCommandService.h"

#include "Commands/CustomCommandBase.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

CustomCommandService::CustomCommandService()
{
}

const QString &CustomCommandService::commandType() const
{
    return Commands::CustomCommandBase::typeName();
}

bool CustomCommandService::up(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    if(!command) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "command is 0!";
        return false;
    }

    QSharedPointer<const Commands::CustomCommandBase> customCommand(command.staticCast<const Commands::CustomCommandBase>());

    if(!customCommand) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "customCommand is 0!";
        return false;
    }

    return customCommand->up(context.database());
}

bool CustomCommandService::isUpValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(command);
    Q_UNUSED(context);
    return true;
}

bool CustomCommandService::down(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const
{
    if(!command) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "command is 0!";
        return false;
    }

    QSharedPointer<const Commands::CustomCommandBase> customCommand(command.staticCast<const Commands::CustomCommandBase>());

    if(!customCommand) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "customCommand is 0!";
        return false;
    }

    return customCommand->down(context.database());
}

bool CustomCommandService::isDownValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const
{
    Q_UNUSED(command);
    Q_UNUSED(context);
    return true;
}

} // namespace CommandExecution
