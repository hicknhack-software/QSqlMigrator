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

    virtual QString quoteTableName(const QString &tableName) const = 0;
    virtual QString quoteColumnName(const QString &columnName) const = 0;
    virtual QString quoteString(const QString &string) const = 0;
};

} // namespace Helper

#endif // QUOTESERVICE_H
