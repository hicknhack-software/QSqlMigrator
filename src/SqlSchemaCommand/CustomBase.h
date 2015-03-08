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

#include "SqlSchemaCommand/Command.h"

// conveniance includes help implementations
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

namespace QSqlMigrator {
namespace SqlSchemaCommand {

/// \brief base class for custom commands
class QSQLMIGRATOR_DLL_EXPORT CustomBase : public Command {
public:
    using CustomName = QString;

public:
    explicit CustomBase(CustomName);

    static const CommandName &commandName() Q_DECL_NOEXCEPT;

    const CustomName &customName() const;

    /// apply the custom command to the database
    virtual bool up(const QSqlDatabase &database) const = 0;

    /// apply the reversal of this command to the database
    /// default: fails
    virtual bool down(const QSqlDatabase &database) const;

    /// \return reversed a command using the down method
    CommandPtr reverse(const CommandPtr& selfPtr) const Q_DECL_OVERRIDE Q_DECL_FINAL;

    /// defaults to no error reporting
    void reportErrors(const Reporter &) const Q_DECL_OVERRIDE;

    /// \return basic string containing the customName
    String toString() const Q_DECL_OVERRIDE;

private:
    QString m_customName;
};

inline const CustomBase::CustomName &
CustomBase::customName() const
{
    return m_customName;
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
