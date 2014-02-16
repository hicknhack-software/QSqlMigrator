#include "HelperRepository.h"

namespace Helper {

HelperRepository::HelperRepository(const QuoteService &quoteService, const TypeMapperService &typeMapperService, const ColumnService &columnService, const SqlStructureService &sqlStructureService)
    : m_quoteService(quoteService)
    , m_typeMapperService(typeMapperService)
    , m_columnService(columnService)
    , m_sqlStructureService(sqlStructureService)
{
}

const QuoteService &HelperRepository::quoteService() const
{
    return m_quoteService;
}

const TypeMapperService &HelperRepository::typeMapperService() const
{
    return m_typeMapperService;
}

const ColumnService &HelperRepository::columnService() const
{
    return m_columnService;
}

const SqlStructureService &HelperRepository::sqlStructureService() const
{
    return m_sqlStructureService;
}

} // namespace Helper
