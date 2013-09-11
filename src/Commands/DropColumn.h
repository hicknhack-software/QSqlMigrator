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

class QSQLMIGRATOR_DLL_EXPORT DropColumn : public BaseCommand
{
public:
    explicit DropColumn(const QString &columnName, const QString &tableName);
    explicit DropColumn(const Structure::Column &column, const QString &tableName);

    bool hasColumn() const;
    const Structure::Column &column() const;
    const QString &columnName() const;
    const QString &tableName() const;
    static const QString &typeName();

private:
    const Structure::Column m_column;
    const QString m_columnName;
    const bool m_hasColumn;
    const QString m_tableName;
};

} // namespace Commands

#endif // COMMANDS_DROPCOLUMN_H
