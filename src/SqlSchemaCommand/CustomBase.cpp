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
#include "CustomBase.h"

namespace QSqlMigrator {
namespace SqlSchemaCommand {

CustomBase::CustomBase(CustomName customName)
    : Command(CustomBase::commandName())
    , m_customName(std::move(customName))
{
    if (m_customName.isEmpty()) {
        ::qWarning() << LOG_PREFIX << CustomBase::commandName() << "command with empty name!";
    }
}

const CustomBase::CommandName &
CustomBase::commandName() Q_DECL_NOEXCEPT
{
    static CommandName commandName("Custom");
    return commandName;
}

bool
CustomBase::down(const QSqlDatabase&) const
{
    return false;
}

namespace {
class ReverseCustom : public CustomBase {
public:
    using CustomBasePtr = QSharedPointer<const CustomBase>;

    explicit ReverseCustom(CustomBasePtr original)
        : CustomBase(QString("Reverse - %1").arg(original->customName()))
        , m_original(original)
    {
    }

    bool up(const QSqlDatabase &database) const Q_DECL_OVERRIDE
    {
        return m_original->down(database);
    }

    bool down(const QSqlDatabase &database) const Q_DECL_OVERRIDE
    {
        return m_original->up(database);
    }

    //    CommandPtr reverse(const CommandPtr&) const Q_DECL_OVERRIDE
    //    {
    //        return m_original;
    //    }

    void reportErrors(const Reporter &reporter) const Q_DECL_OVERRIDE
    {
        m_original->reportErrors(reporter);
    }

    String toString() const Q_DECL_OVERRIDE
    {
        return QString("Revere - %1").arg(m_original->toString());
    }

private:
    CustomBasePtr m_original;
};
} // namespace

CustomBase::CommandPtr
CustomBase::reverse(const CommandPtr& selfPtr) const
{
    return QSharedPointer<ReverseCustom>::create(selfPtr.staticCast<const CustomBase>());
}

void
CustomBase::reportErrors(const Reporter &) const
{
}

CustomBase::String
CustomBase::toString() const
{
    return QString("%1 '%2'").arg(commandName()).arg(customName());
}

} // namespace SqlSchemaCommand
} // namespace QSqlMigrator
