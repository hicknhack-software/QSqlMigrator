#ifndef LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONSERVICE_H
#define LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONSERVICE_H

#include "config.h"

namespace LocalSchemeMigrator {

class LocalSchemeComparisonContext;

class QSQLMIGRATOR_DLL_EXPORT LocalSchemeComparisonService
{
public:
    LocalSchemeComparisonService();

    bool compareLocalSchemeWithDatabase(const LocalSchemeComparisonContext &context) const;
};

} // namespace LocalSchemeMigrator

#endif // LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONSERVICE_H
