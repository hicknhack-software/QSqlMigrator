#ifndef HELPER_DBREADERSERVICE_H
#define HELPER_DBREADERSERVICE_H

#include "config.h"

#include <QString>
#include <QSqlDatabase>

namespace Structure {
class Table;
}

namespace CommandExecution {
class CommandExecutionContext;
}

namespace Helper {

class QSQLMIGRATOR_DLL_EXPORT DbReaderService
{
public:
    DbReaderService();
    virtual ~DbReaderService();

    virtual Structure::Table getTableDefinition(const QString &tableName
                                        , QSqlDatabase database) const = 0;
};

} // namespace Helper

#endif // DBREADERSERVICE_H
