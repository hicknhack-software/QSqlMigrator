#include "HelperRepository.h"

namespace Helper {

HelperRepository::HelperRepository()
{
}

void HelperRepository::setColumnService(ColumnService *columnService)
{
    m_columnService.reset(columnService);
}

void HelperRepository::setDbReaderService(DbReaderService *dbReaderService)
{
    m_dbReaderService.reset(dbReaderService);
}

void HelperRepository::setQuoteService(QuoteService *quoteService)
{
    m_quoteService.reset(quoteService);
}

void HelperRepository::setTypeMapperService(TypeMapperService *typeMapperService)
{
    m_typeMapperService.reset(typeMapperService);
}

const ColumnService &HelperRepository::columnService() const
{
    return *m_columnService;
}

const DbReaderService &HelperRepository::dbReaderService() const
{
    return *m_dbReaderService;
}

const QuoteService &HelperRepository::quoteService() const
{
    return *m_quoteService;
}

const TypeMapperService &HelperRepository::typeMapperService() const
{
    return *m_typeMapperService;
}

} // namespace Helper
