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
#ifndef COMMANDS_DROPCOLUMN_H
#define COMMANDS_DROPCOLUMN_H

#include "Commands/BaseCommand.h"
#include "Structure/Column.h"

namespace Commands {

/*!
 * \brief value object representing the command to drop a column
 */
class QSQLMIGRATOR_DLL_EXPORT DropColumn : public BaseCommand
{
public:
    //! Constructs the command to drop a column by name (but cannot restore it)
    explicit DropColumn(const QString &columnName, const QString &tableName);

    //! Constructs the command to drop a column with a column definition, allowing a restore
    explicit DropColumn(const Structure::Column &column, const QString &tableName);

    ~DropColumn() QSQL_OVERRIDE_D {}

    static const QString &typeName();

    const QString &tableName() const;
    const QString &columnName() const;
    const Structure::Column &column() const;

    CommandPtr reverse() const Q_DECL_OVERRIDE;

private:
    const QString m_tableName;
    const Structure::Column m_column;
};

inline const QString &DropColumn::tableName() const
{
    return m_tableName;
}

inline const QString &DropColumn::columnName() const
{
    return m_column.name();
}

inline const Structure::Column &DropColumn::column() const
{
    return m_column;
}

} // namespace Commands

#endif // COMMANDS_DROPCOLUMN_H
