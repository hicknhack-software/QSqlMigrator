#ifndef LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONSERVICE_H
#define LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONSERVICE_H

#include "LocalSchemeMigrator/LocalSchemeComparisonContext.h"

namespace LocalSchemeMigrator {

class QSQLMIGRATOR_DLL_EXPORT LocalSchemeComparisonService
{
public:
    LocalSchemeComparisonService();
    ~LocalSchemeComparisonService();

    //bool compareDatabaseWithLocalScheme(const LocalSchemeComparisonContext &context, QStringList tables, QStringList indexes) const;
    bool compareLocalSchemeWithDatabase(const LocalSchemeComparisonContext &context) const;
};

} // namespace LocalSchemeMigrator

#endif // LOCALSCHEMECOMPARISONSERVICE_H
