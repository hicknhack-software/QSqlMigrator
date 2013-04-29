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
#ifndef MIGRATION_H
#define MIGRATION_H

#include "Commands/BaseCommand.h"

namespace Migrations {

class QSQLMIGRATOR_DLL_EXPORT Migration
{
public:
     explicit Migration();

     Commands::CommandPtrList commandList() const;

     //! ownership für pointer wird übernommen
     void add(Commands::BaseCommand *cmd);
     void add(Commands::CommandPtr cmd);

private:
     Commands::CommandPtrList m_commandList;
};

} // namespace Migrations

#endif // MIGRATION_H
