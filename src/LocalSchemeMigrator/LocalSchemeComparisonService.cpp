#include "LocalSchemeComparisonService.h"

#include "Structure/Table.h"

#include <QDebug>
#include <QStringList>

namespace LocalSchemeMigrator {

LocalSchemeComparisonService::LocalSchemeComparisonService()
{
}

LocalSchemeComparisonService::~LocalSchemeComparisonService()
{
}

//bool LocalSchemeComparisonService::compareDatabaseWithLocalScheme(const LocalSchemeComparisonContext &context, QStringList tables, QStringList indexes) const
//{
//    return true;
//}

bool LocalSchemeComparisonService::compareLocalSchemeWithDatabase(const LocalSchemeComparisonContext &context) const
{
    bool success = true;

    foreach (const Structure::Table &table, context.localScheme()->tables().values()) {
        Structure::Table realTable = context.helperAggregate().dbReaderService->getTableDefinition(table.name(), context.database());
        if (realTable.columns().length() != table.columns().length()) {
            qWarning() << LOG_PREFIX << "columns count doesn't match";
            success = false;
        }
        for (int i=0; i<table.columns().length(); i++) {
            const Structure::Column &realColumn = realTable.columns().at(i);
            const Structure::Column &column = table.columns().at(i);

            if (realColumn.name() != column.name()) {
                qWarning() << LOG_PREFIX << "table" << table.name() << "column" << column.name() << "is not named" << realColumn.name();
                success = false;
            }
            if (realColumn.hasDefaultValue() != column.hasDefaultValue()) {
                if (column.hasDefaultValue())
                    qWarning() << LOG_PREFIX << "table" << table.name() << "column" << column.name() << "has a default value of" << column.defaultValue();
                else
                    qWarning() << LOG_PREFIX << "table" << table.name() << "column" << column.name() << "has not a default value of" << realColumn.defaultValue();
                success = false;
            } else if (realColumn.defaultValue() != column.defaultValue() ){
                qWarning() << LOG_PREFIX << "table" << table.name() << "column" << column.name() << "has a default value of" << column.defaultValue() << ", while real column has one of" << realColumn.defaultValue();
                success = false;
            }
            {
                QString sqlTypeString;
                if (!column.hasSqlTypeString())
                    sqlTypeString = context.helperAggregate().typeMapperService->map(column.sqlType());
                else
                    sqlTypeString = column.sqlTypeString();
                if (realColumn.sqlTypeString() != sqlTypeString) {
                    qWarning() << LOG_PREFIX << "table" << table.name() << "column" << column.name() << "is of type" << sqlTypeString << ", while real column is of type" << realColumn.sqlTypeString();
                    success = false;
                }
            }
            if (realColumn.isAutoIncremented() != column.isAutoIncremented()) {
                qWarning() << LOG_PREFIX << "table" << table.name() << "column" << column.name() << "has auto increment set to" << column.isAutoIncremented() << ", while real column has it set to" << realColumn.isAutoIncremented();
                success = false;
            }
            if (realColumn.isNullable() != column.isNullable()) {
                qWarning() << LOG_PREFIX << "table" << table.name() << "column" << column.name() << "has nullability set to" << column.isNullable() << ", while real column has it set to" << realColumn.isNullable();
                success = false;
            }
            if (realColumn.isPrimary() != column.isPrimary()) {
                qWarning() << LOG_PREFIX << "table" << table.name() << "column" << column.name() << "has primary set to" << column.isPrimary() << ", while real column has it set to" << realColumn.isPrimary();
                success = false;
            }
            if (realColumn.isUnique() != column.isUnique()) {
                qWarning() << LOG_PREFIX << "table" << table.name() << "column" << column.name() << "has uniqueness set to" << column.isUnique() << ", while real column has it set to" << realColumn.isUnique();
                success = false;
            }
        }
    }
    foreach (const Structure::Index &index, context.localScheme()->indexes()) {
        Structure::Index realIndex = context.helperAggregate().dbReaderService->getIndexDefinition(index.name(), index.tableName(), context.database());
        if (index.columns() != realIndex.columns()) {
            qWarning() << LOG_PREFIX << "table" << index.tableName() << "index" << index.name() << "have different columns";
            qWarning() << LOG_PREFIX << "table" << index.tableName() << "local scheme index:" << context.helperAggregate().columnService->generateIndexColumnDefinitionSql(index.columns());
            qWarning() << LOG_PREFIX << "table" << index.tableName() << "real index:" << context.helperAggregate().columnService->generateIndexColumnDefinitionSql(realIndex.columns());
            success = false;
        }
    }

    return success;
}

} // namespace LocalSchemeMigrator
