#ifndef HELPER_QUOTESERVICE_H
#define HELPER_QUOTESERVICE_H

#include "config.h"

#include <QString>

namespace Helper {

class QSQLMIGRATOR_DLL_EXPORT QuoteService
{
public:
    QuoteService();
    virtual ~QuoteService();

    virtual QString quoteTableName(const QString &tableName) = 0;
    virtual QString quoteColumnName(const QString &columnName) = 0;
    virtual QString quoteString(const QString &string) = 0;
};

} // namespace Helper

#endif // QUOTESERVICE_H
