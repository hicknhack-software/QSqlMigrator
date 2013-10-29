#ifndef HELPER_HELPERAGGREGATE_H
#define HELPER_HELPERAGGREGATE_H

#include "Helper/ColumnService.h"
#include "Helper/DbReaderService.h"
#include "Helper/QuoteService.h"
#include "Helper/TypeMapperService.h"

#include "config.h"

#include <QSharedPointer>

namespace Helper {

class HelperRepository;
typedef QSharedPointer< HelperRepository > HelperRepositoryPtr;

//TODO: move this class into src/Helper/
class QSQLMIGRATOR_DLL_EXPORT HelperRepository
{
public:
    HelperRepository();

    void setColumnService(ColumnService *columnService);
    void setDbReaderService(DbReaderService *dbReaderService);
    void setQuoteService(QuoteService *quoteService);
    void setTypeMapperService(TypeMapperService *typeMapperService);

    const ColumnService &columnService() const;
    const DbReaderService &dbReaderService() const;
    const QuoteService &quoteService() const;
    const TypeMapperService &typeMapperService() const;

private:
    QSharedPointer<ColumnService> m_columnService;
    QSharedPointer<DbReaderService> m_dbReaderService;
    QSharedPointer<QuoteService> m_quoteService;
    QSharedPointer<TypeMapperService> m_typeMapperService;
};

} // namespace helper

#endif // HELPER_HELPERAGGREGATE_H
