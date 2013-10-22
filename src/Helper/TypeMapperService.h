#ifndef HELPER_TYPEMAPPERSERVICE_H
#define HELPER_TYPEMAPPERSERVICE_H

#include "config.h"

#include <Structure/Column.h>

#include <QString>
#include <QHash>

namespace Helper {

class QSQLMIGRATOR_DLL_EXPORT TypeMapperService
{
protected:
    QHash<QVariant::Type, QString> typeMap;

public:
    TypeMapperService();
    virtual ~TypeMapperService();

    virtual QString map(const sqlType& type) const = 0;
};

} // namespace Helper

#endif // TYPEMAPPERSERVICE_H
