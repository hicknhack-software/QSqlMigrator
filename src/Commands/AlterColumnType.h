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

#include "Structure/Type.h"

namespace Commands {

/*!
 * \brief value object representing the command to alter the type of a column in a table
 */
class QSQLMIGRATOR_DLL_EXPORT AlterColumnType : public BaseCommand
{
public:
    explicit AlterColumnType(const QString &columnName, const QString &tableName,
                             const Structure::Type &newType, const Structure::Type &oldType);

    explicit AlterColumnType(const QString &columnName, const QString &tableName,
                             const Structure::Type &newType);

    ~AlterColumnType() QSQL_OVERRIDE_D {}

    static const QString &typeName();

    const QString &tableName() const;
    const QString &columnName() const;
    const Structure::Type &newType() const;
    const Structure::Type &oldType() const;

    CommandPtr reverse() const Q_DECL_OVERRIDE;

private:
    const QString m_tableName;
    const QString m_columnName;
    const Structure::Type m_newType;
    const Structure::Type m_oldType;
};

inline const QString &AlterColumnType::tableName() const
{
    return m_tableName;
}

inline const QString &AlterColumnType::columnName() const
{
    return m_columnName;
}

inline const Structure::Type &AlterColumnType::newType() const
{
    return m_newType;
}

inline const Structure::Type &AlterColumnType::oldType() const
{
    return m_oldType;
}

} // namespace Commands

#endif // COMMANDS_ALTERCOLUMNTYPE_H
