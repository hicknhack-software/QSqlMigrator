#include "LocalScheme.h"

namespace Structure {

LocalScheme::LocalScheme()
{
}

QMap<QString, Table> &LocalScheme::tables()
{
    return m_tables;
}

QMap<QString, Index> &LocalScheme::indexes()
{
    return m_indexes;
}

} // namespace Structure
