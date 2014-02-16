#ifndef LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONCONTEXT_H
#define LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONCONTEXT_H

#include "config.h"

#include "Structure/LocalScheme.h"
#include "Helper/HelperRepository.h"

#include <QSqlDatabase>

namespace LocalSchemeMigrator {

class QSQLMIGRATOR_DLL_EXPORT LocalSchemeComparisonContext
{
public:
    LocalSchemeComparisonContext(const LocalSchemePtr &localScheme,
                                 const Helper::HelperRepository &helperRepository,
                                 const QSqlDatabase &database);

    const LocalSchemePtr &localScheme() const;
    const Helper::HelperRepository &helperRepository() const;
    const QSqlDatabase &database() const;

private:
    const LocalSchemePtr m_localScheme;
    const Helper::HelperRepository m_helperRepository;
    const QSqlDatabase m_database;
};

}

#endif // LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONCONTEXT_H
