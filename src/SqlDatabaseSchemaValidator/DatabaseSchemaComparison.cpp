/****************************************************************************
**
** Copyright (C) 2013, HicknHack Software
** All rights reserved.
** Contact: http://www.hicknhack-software.com/contact
**
** This file is part of the QSqlMigrator
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/
#include "DatabaseSchemaComparison.h"

#include "SqlDatabaseAdapter/ColumnMapper.h"
#include "SqlDatabaseAdapter/SchemaReflection.h"
#include "SqlDatabaseAdapter/ValueTypeMapper.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaValidator {

bool
DatabaseSchemaComparison::reportMissingSchemaParts(const Schema &schema,
                                                   const Adapter &adapter,
                                                   const Reporter &reporter) const
{
    auto success = true;
    const auto schemaReflection = adapter.schemaReflection();
    const auto columnMapper = adapter.columnMapper();
    const auto typeMapper = adapter.valueTypeMapper();

    const auto scope = reporter->scope([&]() { return QStringLiteral("Schema Comparison"); });
    Q_UNUSED(scope);

    for (const auto &table : schema.tables()) {
        const auto tableScope = reporter->scope([&]() { return QString("Table %1").arg(table.name()); });
        Q_UNUSED(tableScope);

        const auto databaseTable = schemaReflection->getTableDefinition(table.name(), adapter.database());
        if (!databaseTable.isValid()) {
            reporter->error("missing table in database");
            success = false;
            continue;
        }
        auto extraDatabaseColumns = databaseTable.columnNames();

        for (const auto &column : table.columns()) {
            const auto columnScope = reporter->scope([&]() { return QString("Column %1").arg(column.name()); });
            Q_UNUSED(columnScope);

            if (!databaseTable.hasColumn(column.name())) {
                reporter->error("missing column in database");
                success = false;
                continue;
            }
            extraDatabaseColumns.removeOne(column.name());
            const auto &databaseColumn = databaseTable.column(column.name());

            if (databaseColumn.hasDefaultValue() != column.hasDefaultValue()) {
                if (column.hasDefaultValue())
                    reporter->error(
                        QString("database column is missing the default value: '%1'").arg(column.defaultValue()));
                else
                    reporter->error("database column has a superfluous default value");
                success = false;
            }
            else if (databaseColumn.defaultValue() != column.defaultValue()) {
                reporter->error(QString("database column has a default value: '%1' but '%2' is required")
                                    .arg(databaseColumn.defaultValue())
                                    .arg(column.defaultValue()));
                success = false;
            }

            {
                const auto sqlTypeString = typeMapper->map(column.type());
                if (0 != QString::compare(sqlTypeString, databaseColumn.type().string(), Qt::CaseInsensitive)) {
                    reporter->error(QString("database type mismatches: database '%1' and schema '%2'")
                                        .arg(databaseColumn.type().string())
                                        .arg(sqlTypeString));
                    success = false;
                }
            }

            if (databaseColumn.isAutoIncremented() != column.isAutoIncremented()) {
                reporter->error(QString("column flag for autoincrement mismatches: database '%1' != schema '%2'")
                                    .arg(databaseColumn.isAutoIncremented())
                                    .arg(column.isAutoIncremented()));
                success = false;
            }
            if (databaseColumn.isNullable() != column.isNullable()) {
                reporter->error(QString("column flag for null allowed mismatches: database '%1' != schema '%2'")
                                    .arg(databaseColumn.isNullable())
                                    .arg(column.isNullable()));
                success = false;
            }
            if (databaseColumn.isPrimary() != column.isPrimary()) {
                reporter->error(QString("column flag for primary key mismatches: database '%1' != schema '%2'")
                                    .arg(databaseColumn.isPrimary())
                                    .arg(column.isPrimary()));
                success = false;
            }
            if (databaseColumn.isUnique() != column.isUnique()) {
                reporter->error(QString("column flag for uniqueness mismatches: database '%1' != schema '%2'")
                                    .arg(databaseColumn.isUnique())
                                    .arg(column.isUnique()));
                success = false;
            }
        }
        for (const auto &extraColumn : extraDatabaseColumns) {
            reporter->error(QString("database has unused column '%1'").arg(extraColumn));
            success = false;
        }
    }
    for (const auto &index : schema.indices()) {
        const auto indexScope = reporter->scope(
            [&]() { return QString("Index '%1' for table '%2'").arg(index.name()).arg(index.tableName()); });
        Q_UNUSED(indexScope);

        const auto databaseIndex =
            schemaReflection->getIndexDefinition(index.name(), index.tableName(), adapter.database());
        if (!databaseIndex.isValid()) {
            reporter->error("missing index in database");
            success = false;
            continue;
        }
        if (index.columns() != databaseIndex.columns()) {
            reporter->error(QString("columns are different: database: '%1' != schema: '%2'")
                                .arg(columnMapper->generateIndexColumnsDefinitionSql(databaseIndex.columns()))
                                .arg(columnMapper->generateIndexColumnsDefinitionSql(index.columns())));
            success = false;
        }
    }

    return success;
}

} // namespace SqlDatabaseSchemaValidator
} // namespace QSqlMigrator
