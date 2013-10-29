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
    QSharedPointer<ColumnService> columnService;
    QSharedPointer<DbReaderService> dbReaderService;
    QSharedPointer<QuoteService> quoteService;
    QSharedPointer<TypeMapperService> typeMapperService;

    HelperRepository();
};

} // namespace helper

#endif // HELPER_HELPERAGGREGATE_H
