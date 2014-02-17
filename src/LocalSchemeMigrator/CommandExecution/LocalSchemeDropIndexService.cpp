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
#include "LocalSchemeMigrator/CommandExecution/LocalSchemeDropIndexService.h"

#include "Commands/DropIndex.h"

#include <QDebug>
#include <QStringList>

namespace CommandExecution {

LocalSchemeDropIndexService::LocalSchemeDropIndexService()
{
}

const QString &LocalSchemeDropIndexService::commandType() const
{
    return Commands::DropIndex::typeName();
}

bool LocalSchemeDropIndexService::execute(const Commands::ConstCommandPtr &command,
                                          CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    QSharedPointer<const Commands::DropIndex> dropIndex(command.staticCast<const Commands::DropIndex>());

    context.localScheme()->dropIndex(dropIndex->name());

    return true;
}

bool LocalSchemeDropIndexService::isValid(const Commands::ConstCommandPtr &command,
                                          const CommandExecution::LocalSchemeCommandExecutionContext &context) const
{
    Q_UNUSED(command);
    Q_UNUSED(context);

    //TODO check for index existence?!

    return true;
}

} // namespace CommandExecution
