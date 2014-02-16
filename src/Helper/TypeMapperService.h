#ifndef HELPER_TYPEMAPPERSERVICE_H
#define HELPER_TYPEMAPPERSERVICE_H

#include "config.h"

#include "Structure/Type.h"

#include <QString>
#include <QHash>

namespace Helper {

/*!
 * \brief interface for a mapping basic types to proper sql types
 */
class QSQLMIGRATOR_DLL_EXPORT TypeMapperService
{
public:
    virtual ~TypeMapperService() {}

    virtual QString map(const Structure::Type& type) const = 0;
};

} // namespace Helper

#endif // TYPEMAPPERSERVICE_H
