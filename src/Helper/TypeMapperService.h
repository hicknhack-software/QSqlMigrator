#ifndef HELPER_TYPEMAPPERSERVICE_H
#define HELPER_TYPEMAPPERSERVICE_H

#include "config.h"

#include <Structure/SqlType.h>

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

    virtual QString map(const SqlType& type) const = 0;
};

} // namespace Helper

#endif // TYPEMAPPERSERVICE_H
