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
#ifndef COMMANDS_DROPINDEX_H
#define COMMANDS_DROPINDEX_H

#include "Commands/BaseCommand.h"
#include "Structure/Index.h"

namespace Commands {

/*!
 * \brief value object representing the command to drop an index
 */
class QSQLMIGRATOR_DLL_EXPORT DropIndex : public BaseCommand
{
public:
    explicit DropIndex(const QString &name);
    explicit DropIndex(const Structure::Index &index);
    ~DropIndex() QSQL_OVERRIDE_D {}

    static const QString &typeName();

    const Structure::Index &index() const;
    const QString &name() const;

    CommandPtr reverse() const Q_DECL_OVERRIDE;

private:
    const Structure::Index m_index;
};

inline const Structure::Index &DropIndex::index() const
{
    return m_index;
}

inline const QString &DropIndex::name() const
{
    return m_index.name();
}

} // namespace Commands

#endif // COMMANDS_DROPINDEX_H
