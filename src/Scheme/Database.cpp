#include "Database.h"

#include <QDebug>

namespace Scheme {

Database &Database::add(const DbTable &newTable)
{
    foreach(const DbTable &table, m_tables) {
        if(table.name() == newTable.name()) {
            ::qDebug() << LOG_PREFIX << Q_FUNC_INFO << "Table" << newTable.name() << "already contained in database!";
            return (*this);
        }
    }

    m_tables << newTable;
    return (*this);
}

Database &Database::removeTable(const QString &tableName)
{
    QMutableListIterator<DbTable> tableIterator(m_tables);
    while(tableIterator.hasNext()) {
        tableIterator.next();
        const DbTable &table = tableIterator.value();
        if(table.name() == tableName) {
            tableIterator.remove();
            break;
        }
    }

    return (*this);
}

}
