#include "LocalSchemeComparisonContext.h"

namespace LocalSchemeMigrator {

LocalSchemeComparisonContext::LocalSchemeComparisonContext()
{
}

void LocalSchemeComparisonContext::setLocalScheme(const LocalSchemePtr &localScheme)
{
    m_localScheme = localScheme;
}

void LocalSchemeComparisonContext::setHelperRepository(const Helper::HelperRepository &helperRepository)
{
    m_helperRepository = helperRepository;
}

void LocalSchemeComparisonContext::setDatabase(const QSqlDatabase &database)
{
    m_database = database;
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
