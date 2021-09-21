#include "SqliteTypeMapperService.h"
namespace Helper {

SqliteTypeMapperService::SqliteTypeMapperService()
{
    using namespace Structure;

    m_typeMap.insert(Type::DateTime,  "Text");
}

QString SqliteTypeMapperService::map(const Structure::Type &type) const
{
    using namespace Structure;

    switch (type.base())
    {
    case Type::BigInt:
        return m_typeMap[Type::BigInt].arg(type.precision(20));
    default:
        return BaseSqlTypeMapperService::map(type);
    }
}
}
