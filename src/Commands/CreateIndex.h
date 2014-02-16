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
#ifndef COMMANDS_CREATEINDEX_H
#define COMMANDS_CREATEINDEX_H

#include "Commands/BaseCommand.h"
#include "Structure/Index.h"

namespace Commands {

/*!
 * \brief value object representing the command to create an index
 */
class QSQLMIGRATOR_DLL_EXPORT CreateIndex : public BaseCommand
{
public:
    explicit CreateIndex(const Structure::Index &index);
    ~CreateIndex() QSQL_OVERRIDE_D {}

    static const QString &typeName();

    const Structure::Index &index() const;

    CommandPtr reverse() const Q_DECL_OVERRIDE;

private:
    const Structure::Index m_index;
};

inline const Structure::Index &CreateIndex::index() const
{
    return m_index;
}

} // namespace Commands

#endif // COMMANDS_CREATEINDEX_H
