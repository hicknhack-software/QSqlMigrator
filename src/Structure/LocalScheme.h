#ifndef STRUCTURE_LOCALSCHEME_H
#define STRUCTURE_LOCALSCHEME_H

#include "config.h"

#include "Structure/Table.h"
#include "Structure/Index.h"

#include <QList>
#include <QSharedPointer>

namespace Structure {

class QSQLMIGRATOR_DLL_EXPORT LocalScheme
{
public:
    LocalScheme();
    QMap<QString, Table> &tables();
    QMap<QString, Index> &indexes();

private:
    QMap<QString, Table> m_tables;
    QMap<QString, Index> m_indexes;
};

} // namespace Structure

typedef QSharedPointer<Structure::LocalScheme> LocalSchemePtr;

#endif // STRUCTURE_LOCALSCHEME_H
