#ifndef STRUCTURE_SQLTYPE_H
#define STRUCTURE_SQLTYPE_H

#include "config.h"

#include <QVariant>

class QSQLMIGRATOR_DLL_EXPORT SqlType
{
public:
    SqlType();
    SqlType(QVariant::Type t, int precision=0, int scale=0);

    QVariant::Type type() const;
    int precision() const;
    int scale() const;

private:
    QVariant::Type m_type;
    int m_precision;
    int m_scale;
};

#endif // STRUCTURE_SQLTYPE_H
