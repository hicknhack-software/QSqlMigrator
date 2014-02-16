#include "LocalSchemeComparisonContext.h"

namespace LocalSchemeMigrator {

LocalSchemeComparisonContext::LocalSchemeComparisonContext(const LocalSchemePtr &localScheme, const Helper::HelperRepository &helperRepository, const QSqlDatabase &database)
    : m_localScheme(localScheme)
    , m_helperRepository(helperRepository)
    , m_database(database)
{
}

const LocalSchemePtr &LocalSchemeComparisonContext::localScheme() const
{
    return m_localScheme;
}

const Helper::HelperRepository &LocalSchemeComparisonContext::helperRepository() const
{
    return m_helperRepository;
}

const QSqlDatabase &LocalSchemeComparisonContext::database() const
{
    return m_database;
}

}
