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
#ifndef HELPER_BASESQLDBREADERSERVICE_H
#define HELPER_BASESQLDBREADERSERVICE_H

#include "config.h"

#include "Helper/DbReaderService.h"

#include <QString>
#include <QSqlDatabase>

namespace Structure {
class Table;
}

namespace CommandExecution {
class CommandExecutionContext;
}

namespace Helper {

class QSQLMIGRATOR_DLL_EXPORT BaseSqlDbReaderService : public DbReaderService
{
public:
    BaseSqlDbReaderService();
    virtual ~BaseSqlDbReaderService() {};

    Structure::Table getTableDefinition(const QString &tableName
                                        , QSqlDatabase database) const Q_DECL_OVERRIDE;
};

} // namespace Helper

#endif // HELPER_BASESQLDBREADER_H
