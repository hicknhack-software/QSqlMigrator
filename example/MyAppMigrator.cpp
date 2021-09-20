#include "MyAppMigrator.h"

#include "M201503301340654_CreateUsers.h"

#include "Migrations/MigrationRepository.h"
#include "QSqlMigrator/QSqlMigratorService.h"
#include "MigrationExecution/MigrationExecutionContext.h"
#include "SqliteMigrator/SqliteMigrator.h"

using namespace Migrations;
using namespace MigrationExecution;

MigrationRepository::NameMigrationMap migrations() {
    MigrationRepository::NameMigrationMap result;
    result.insert("M201503301340654_CreateUsers", new M201503301340654_CreateUsers());
    return result;
}

bool MyAppMigrator::migrate(QSqlDatabase database) {
    auto contextBuilder = MigrationExecutionContext::Builder(migrations());
    contextBuilder.setDatabase(database);
    auto context = SqliteMigrator::buildContext(contextBuilder);

    QSqlMigrator::QSqlMigratorService manager(*context);
    return manager.applyAll();
}

#include "MigrationExecution/LocalSchemeMigrationExecutionContext.h"
#include "MigrationExecution/LocalSchemeMigrationExecutionService.h"
#include "LocalSchemeMigrator/LocalSchemeMigrator.h"
#include "LocalSchemeMigrator/LocalSchemeComparisonContext.h"
#include "LocalSchemeMigrator/LocalSchemeComparisonService.h"

using namespace LocalSchemeMigrator;

bool MyAppMigrator::validate(QSqlDatabase database)
{
    LocalSchemePtr localScheme(new Structure::LocalScheme);
    auto local = LocalSchemeMigrationExecutionContext(migrations());
    local.setLocalScheme(localScheme);
    local.setLocalSchemeCommandServiceRepository(createCommandServiceRepository());

    LocalSchemeMigrationExecutionService localManager;
    localManager.executeBatch(local.migrationMap().keys(), local);

    auto contextBuilder = MigrationExecutionContext::Builder(migrations());
    contextBuilder.setDatabase(database);
    auto context = SqliteMigrator::buildContext(contextBuilder);

    LocalSchemeComparisonContext comparisonContext(localScheme, context->helperRepository(), database);
    LocalSchemeComparisonService comparisonService;
    return comparisonService.compareLocalSchemeWithDatabase(comparisonContext);
}
