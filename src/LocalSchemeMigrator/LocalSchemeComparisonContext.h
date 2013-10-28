#ifndef LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONCONTEXT_H
#define LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONCONTEXT_H

#include "config.h"

#include "Structure/LocalScheme.h"
#include "Helper/HelperAggregate.h"

#include <QSqlDatabase>

namespace LocalSchemeMigrator {

class QSQLMIGRATOR_DLL_EXPORT LocalSchemeComparisonContext
{
public:
    LocalSchemeComparisonContext();

    void setLocalScheme(const LocalSchemePtr &localScheme);
    void setHelperAggregate(const Helper::HelperAggregate &helperAggregate);
    void setDatabase(const QSqlDatabase &database);
    const LocalSchemePtr &localScheme() const;
    const Helper::HelperAggregate &helperAggregate() const;
    const QSqlDatabase &database() const;

private:
    LocalSchemePtr m_localScheme;
    Helper::HelperAggregate m_helperAggregate;
    QSqlDatabase m_database;
};

}

#endif // LOCALSCHEMEMIGRATOR_LOCALSCHEMECOMPARISONCONTEXT_H
