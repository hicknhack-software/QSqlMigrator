#ifndef HELPER_COLUMNSERVICE_H
#define HELPER_COLUMNSERVICE_H

#include "config.h"

#include "Structure/Column.h"
#include "Structure/Index.h"

#include <QString>
#include <QHash>

namespace Helper {

class QSQLMIGRATOR_DLL_EXPORT ColumnService
{
public:
    ColumnService();
    virtual ~ColumnService();

    virtual QString generateColumnDefinitionSql(const Structure::Column &column) const = 0;
    virtual QString generateColumnDefinitionSql(const QList<Structure::Column> &columnList) const = 0;

    virtual QString generateIndexColumnDefinitionSql(const QString &columnName
                                                     , const Structure::Index::SortOrder &sortOrder) const = 0;
    virtual QString generateIndexColumnDefinitionSql(const Structure::Index::IndexHash &columns) const = 0;
};

} // namespace Helper

#endif // COLUMNSERVICE_H
