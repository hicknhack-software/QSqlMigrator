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
    LocalSchemeComparisonContext();

    void setLocalScheme(const LocalSchemePtr &localScheme);
    void setHelperRepository(const Helper::HelperRepository &helperRepository);
    void setDatabase(const QSqlDatabase &database);
    const LocalSchemePtr &localScheme() const;
    const Helper::HelperRepository &helperRepository() const;
    const QSqlDatabase &database() const;

private:
    LocalSchemePtr m_localScheme;
    Helper::HelperRepository m_helperRepository;
    QSqlDatabase m_database;
};

}

#endif // LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONCONTEXT_H
