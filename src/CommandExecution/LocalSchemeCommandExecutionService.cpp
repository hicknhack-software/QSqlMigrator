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
#include "LocalSchemeCommandExecutionService.h"

#include <QDebug>

namespace CommandExecution {

bool LocalSchemeCommandExecutionService::execute(const Commands::CommandPtr command
                                      , LocalSchemeCommandServiceRepositoryPtr serviceRepository
                                      , LocalSchemeCommandExecutionContext &context) const
{
    if(!command) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "command is 0!";
        return false;
    }

    if(!serviceRepository) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "serviceRepository is 0!";
        return false;
    }

    LocalSchemeBaseCommandServicePtr service = serviceRepository->getService( command->name() );
    if(!service) {
        ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "service is 0!";
        return false;
    }

    if (service->isValid(command, context)) {
        return service->execute(command, context);
    }

    return false;
}

bool LocalSchemeCommandExecutionService::batch(const Commands::CommandPtrList &commandList
                                    , LocalSchemeCommandServiceRepositoryPtr serviceRepository
                                    , LocalSchemeCommandExecutionContext &context) const
{
    foreach (Commands::CommandPtr command, commandList) {
        if(!command) {
            ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "command is 0!";
            return false;
        }
        bool isSuccess = this->execute(command, serviceRepository, context);
        if (!isSuccess) {
            return false; // last command failed
        }
    }
    return true;
}

} // namespace CommandExecution
