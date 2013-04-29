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
#ifndef MYSQLCOMMANDSERVICES_MYSQLDROPTABLESERVICE_H
#define MYSQLCOMMANDSERVICES_MYSQLDROPTABLESERVICE_H

#include "CommandExecution/BaseCommandExecutionService.h"

namespace CommandExecution {

class MysqlDropTableService : public CommandExecution::BaseCommandExecutionService
{
public:
     MysqlDropTableService();

     const QString &commandType() const;

     bool up(const Commands::ConstCommandPtr &command, CommandExecutionContext &context) const;
     bool isUpValid(const Commands::ConstCommandPtr &command, const CommandExecutionContext &context) const;

     bool down(const Commands::ConstCommandPtr &command, CommandExecutionContext &context) const;
     bool isDownValid(const Commands::ConstCommandPtr &command, const CommandExecutionContext &context) const;
};

} // namespace CommandExecution

#endif // MYSQLCOMMANDSERVICES_MYSQLDROPTABLESERVICE_H
