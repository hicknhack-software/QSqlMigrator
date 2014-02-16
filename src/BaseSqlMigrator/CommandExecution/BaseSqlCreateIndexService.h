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
#ifndef COMMANDEXECUTION_BASESQLCREATEINDEXSERVICE_H
#define COMMANDEXECUTION_BASESQLCREATEINDEXSERVICE_H

#include "CommandExecution/BaseCommandExecutionService.h"

namespace Commands {
class CreateIndex;
} // namespace Commands

namespace CommandExecution {

class QSQLMIGRATOR_DLL_EXPORT BaseSqlCreateIndexService : public BaseCommandExecutionService
{
public:
    BaseSqlCreateIndexService();

    const QString &commandType() const;

    static bool execute(const Commands::CreateIndex &createIndex, const CommandExecution::CommandExecutionContext &context);

    bool execute(const Commands::ConstCommandPtr &command, CommandExecution::CommandExecutionContext &context) const;
    bool isValid(const Commands::ConstCommandPtr &command, const CommandExecution::CommandExecutionContext &context) const;
};

} // namespace CommandExecution

#endif // COMMANDEXECUTION_BASESQLCREATEINDEXSERVICE_H
