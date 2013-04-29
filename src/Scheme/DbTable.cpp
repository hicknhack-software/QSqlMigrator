#include "DbTable.h"

#include <QDebug>

namespace Scheme {

DbTable::DbTable(const QString &name)
    : m_name(name)
{
}

DbTable::DbTable(const QString &name, const QList<DbColumn> &columns)
    : m_name(name)
    , m_columns(columns)
{
}

QList<DbColumn> &DbTable::columns()
{
    return const_cast<QList<DbColumn> &>(static_cast<const DbTable &>(*this).columns());
}

const QList<DbColumn> &DbTable::columns() const
{
    return m_columns;
}

const QString &DbTable::name() const
{
    return m_name;
}

void DbTable::setName(const QString &newName)
{
    m_name = newName;
}

DbTable &DbTable::add(const DbColumn &newColumn)
{
    foreach(const DbColumn &column, m_columns) {
        if(column.name() == newColumn.name()) {
            ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "Table already contains column" << newColumn.name();
            return (*this);
        }
    }

    m_columns << newColumn;
    return (*this);
}

DbTable &DbTable::removeColumn(const QString &columnName)
{
    QMutableListIterator<DbColumn> columnIterator(m_columns);
    while(columnIterator.hasNext()) {
        columnIterator.next();
        const DbColumn &column = columnIterator.value();
        if(column.name() == columnName) {
            columnIterator.remove();
            break;
        }
    }

    return (*this);
}

} // namespace Scheme
