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
#pragma once

#include "SqlDatabaseSchemaAdapter/CommandExecutor.h"

#include "config.h"

#include <QSharedPointer>
#include <QString>
#include <QList>

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

/// \brief a repository storing a command executor for each kind of command
class QSQLMIGRATOR_DLL_EXPORT CommandExecutorRepository {
public:
    using CommandName = QString;
    using CommandExecutor = QSharedPointer<QSqlMigrator::SqlDatabaseSchemaAdapter::CommandExecutor>;
    using CommandExecutors = QList<CommandExecutor>;

public:
    /// \return the stored executor for the command name or null pointer
    CommandExecutor findExecutor(const CommandName &) const;

    void add(CommandExecutor);

private:
    CommandExecutors m_commandExecutors;
};

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator
