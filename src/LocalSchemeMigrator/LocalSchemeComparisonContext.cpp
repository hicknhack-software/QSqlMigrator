#include "LocalSchemeComparisonContext.h"

namespace LocalSchemeMigrator {

LocalSchemeComparisonContext::LocalSchemeComparisonContext()
{
}

void LocalSchemeComparisonContext::setLocalScheme(const LocalSchemePtr &localScheme)
{
    m_localScheme = localScheme;
}

void LocalSchemeComparisonContext::setHelperAggregate(const Helper::HelperAggregate &helperAggregate)
{
    m_helperAggregate = helperAggregate;
}

void LocalSchemeComparisonContext::setDatabase(const QSqlDatabase &database)
{
    m_database = database;
}

const LocalSchemePtr &LocalSchemeComparisonContext::localScheme() const
{
    return m_localScheme;
}

const Helper::HelperAggregate &LocalSchemeComparisonContext::helperAggregate() const
{
    return m_helperAggregate;
}

const QSqlDatabase &LocalSchemeComparisonContext::database() const
{
    return m_database;
}

}
