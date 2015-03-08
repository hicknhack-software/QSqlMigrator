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

#include "config.h"

#include <QString>
#include <QSharedPointer>

namespace QSqlMigrator {
namespace LoggingTrace {
class Logging;
} // namespace LoggingTrace

namespace SqlSchemaCommand {

/// \brief abstract class for value object representing all kinds of commands
class QSQLMIGRATOR_DLL_EXPORT Command {
public:
    using CommandName = QString;
    using CommandPtr = QSharedPointer<const Command>;
    using String = QString;
    using Reporter = QSharedPointer<LoggingTrace::Logging>;

public:
    explicit Command(CommandName);

    /// \return name of the instance command
    const CommandName &commandName() const Q_DECL_NOEXCEPT;

    /// \return reverting command or invalid command
    /// \arg selfPtr is only used by CustomBase commands and allows to return a self reference
    virtual CommandPtr reverse(const CommandPtr& selfPtr = {}) const = 0;

    /// check and report all errors to the reporter
    virtual void reportErrors(const Reporter &) const = 0;

    /// \return a string representation of the command
    virtual String toString() const = 0;

private:
    CommandName m_typeName;
};

inline const Command::CommandName &
Command::commandName() const Q_DECL_NOEXCEPT
{
    return m_typeName;
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
