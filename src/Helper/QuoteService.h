#ifndef HELPER_QUOTESERVICE_H
#define HELPER_QUOTESERVICE_H

#include "config.h"

#include <QString>

namespace Helper {

/*!
 * \brief interface for a service that is used to quote different kinds of strings for a database
 */
class QSQLMIGRATOR_DLL_EXPORT QuoteService
{
public:
    virtual ~QuoteService() {}

    //! \return the table name properly quoted for an sql execution
    virtual QString quoteTableName(const QString &tableName) const = 0;

    //! \return the column name properly quoted for an sql execution
    virtual QString quoteColumnName(const QString &columnName) const = 0;

    //! \return the string value properly quoted for an sql execution
    virtual QString quoteString(const QString &string) const = 0;
};

} // namespace Helper

#endif // QUOTESERVICE_H
