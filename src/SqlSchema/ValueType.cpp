#include "ValueType.h"

namespace QSqlMigrator {
namespace SqlSchema {

const ValueType &ValueType::invalid() Q_DECL_NOEXCEPT
{
    static ValueType s_invalid{Type::Invalid};
    return s_invalid;
}

} // namespace SqlSchema
} // namespace QSqlMigrator
