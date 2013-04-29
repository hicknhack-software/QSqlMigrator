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
#ifndef COMMANDS_ALTERCOLUMNTYPE_H
#define COMMANDS_ALTERCOLUMNTYPE_H

#include "Commands/BaseCommand.h"

namespace Commands {

class QSQLMIGRATOR_DLL_EXPORT AlterColumnType : public BaseCommand
{
public:
    explicit AlterColumnType(const QString &columnName, const QString &tableName
                             , const QString &newType, const QString &oldType = "");

    const bool hasOldType() const;
    const QString &columnName() const;
    const QString &newType() const;
    const QString &oldType() const;
    const QString &tableName() const;
    static const QString &typeName();

private:
    const QString m_tableName;
    const QString m_columnName;
    const QString m_newType;
    const QString m_oldType;
};

} // namespace Commands

#endif // COMMANDS_ALTERCOLUMNTYPE_H
