#ifndef HELPER_HELPERREPOSITORY_H
#define HELPER_HELPERREPOSITORY_H

#include "config.h"

namespace Helper {

class QuoteService;
class TypeMapperService;
class ColumnService;
class SqlStructureService;

/*!
 * \brief a simple repository of all helper classes
 */
class QSQLMIGRATOR_DLL_EXPORT HelperRepository
{
public:
    HelperRepository(const QuoteService &quoteService,
                     const TypeMapperService &typeMapperService,
                     const ColumnService &columnService,
                     const SqlStructureService &sqlStructureService);

    const QuoteService &quoteService() const;
    const TypeMapperService &typeMapperService() const;
    const ColumnService &columnService() const;
    const SqlStructureService &sqlStructureService() const;

private:
    const QuoteService &m_quoteService;
    const TypeMapperService &m_typeMapperService;
    const ColumnService &m_columnService;
    const SqlStructureService &m_sqlStructureService;
};

} // namespace helper

#endif // HELPER_HELPERREPOSITORY_H
