DynamicLibrary {
    name: "QSqlMigrator"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "sql"] }
    cpp.includePaths: ["./"]
    cpp.defines: ["LOG_PREFIX=\"[QSqlMigrator]\"",
                  "_BUILDING_QSQLMIGRATOR_DLL"]
    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core", "sql"] }
        cpp.includePaths: ["./"]
    }

    Group {
        name: "api"

        files: [
            "api.h",
            "config.h"
        ]
    }

    Group {
       name: "BaseSqlMigrator/CommandExecution"
       prefix: "BaseSqlMigrator/CommandExecution/"

       files: [
           "BaseSqlAddColumnService.cpp",
           "BaseSqlAddColumnService.h",
           "BaseSqlAlterColumnTypeService.cpp",
           "BaseSqlAlterColumnTypeService.h",
           "BaseSqlCreateIndexService.cpp",
           "BaseSqlCreateIndexService.h",
           "BaseSqlCreateTableService.cpp",
           "BaseSqlCreateTableService.h",
           "BaseSqlDropColumnService.cpp",
           "BaseSqlDropColumnService.h",
           "BaseSqlDropIndexService.cpp",
           "BaseSqlDropIndexService.h",
           "BaseSqlDropTableService.cpp",
           "BaseSqlDropTableService.h",
           "BaseSqlRenameColumnService.cpp",
           "BaseSqlRenameColumnService.h",
           "BaseSqlRenameTableService.cpp",
           "BaseSqlRenameTableService.h",
        ]
    }
    Group {
        name: "BaseSqlMigrator/Helper"
        prefix: "BaseSqlMigrator/Helper/"

        files: [
            "BaseSqlColumnService.cpp",
            "BaseSqlColumnService.h",
            "BaseSqlQuoteService.cpp",
            "BaseSqlQuoteService.h",
            "BaseSqlTypeMapperService.cpp",
            "BaseSqlTypeMapperService.h",
        ]
    }

    Group {
        name: "BaseSqlMigrator/MigrationTracker"
        prefix: "BaseSqlMigrator/MigrationTracker/"

        files: [
            "BaseMigrationTableService.cpp",
            "BaseMigrationTableService.h",
        ]
    }

    Group {
        name: "Commands"
        prefix: "Commands/"

        files: [
            "AddColumn.cpp",
            "AddColumn.h",
            "AlterColumnType.cpp",
            "AlterColumnType.h",
            "BaseCommand.cpp",
            "BaseCommand.h",
            "CreateIndex.cpp",
            "CreateIndex.h",
            "CreateTable.cpp",
            "CreateTable.h",
            "CustomCommandBase.cpp",
            "CustomCommandBase.h",
            "DropColumn.cpp",
            "DropColumn.h",
            "DropIndex.cpp",
            "DropIndex.h",
            "DropTable.cpp",
            "DropTable.h",
            "RenameColumn.cpp",
            "RenameColumn.h",
            "RenameTable.cpp",
            "RenameTable.h",
        ]
    }

    Group {
        name: "CommandExecution"
        prefix: "CommandExecution/"

        files: [
            "BaseCommandExecutionService.cpp",
            "BaseCommandExecutionService.h",
            "CommandExecutionContext.cpp",
            "CommandExecutionContext.h",
            "CommandExecutionService.cpp",
            "CommandExecutionService.h",
            "CommandExecutionServiceRepository.cpp",
            "CommandExecutionServiceRepository.h",
            "CustomCommandService.cpp",
            "CustomCommandService.h",
            "LocalSchemeBaseCommandExecutionService.cpp",
            "LocalSchemeBaseCommandExecutionService.h",
            "LocalSchemeCommandExecutionContext.cpp",
            "LocalSchemeCommandExecutionContext.h",
            "LocalSchemeCommandExecutionService.cpp",
            "LocalSchemeCommandExecutionService.h",
            "LocalSchemeCommandExecutionServiceRepository.cpp",
            "LocalSchemeCommandExecutionServiceRepository.h",
        ]
    }

    Group {
        name: "Helper"
        prefix: "Helper/"

        files: [
            "ColumnService.h",
            "HelperRepository.cpp",
            "HelperRepository.h",
            "QuoteService.h",
            "SqlStructureService.h",
            "TypeMapperService.h",
        ]
    }

    Group {
        name: "LocalSchemeMigrator"
        prefix: "LocalSchemeMigrator/"

        files: [
            "LocalSchemeComparisonContext.cpp",
            "LocalSchemeComparisonContext.h",
            "LocalSchemeComparisonService.cpp",
            "LocalSchemeComparisonService.h",
            "LocalSchemeMigrator.cpp",
            "LocalSchemeMigrator.h",
        ]

        Group {
            name: "LocalSchemeMigrator/CommandExecution"
            prefix: "LocalSchemeMigrator/CommandExecution/"

            files: [
                "LocalSchemeAddColumnService.cpp",
                "LocalSchemeAddColumnService.h",
                "LocalSchemeAlterColumnTypeService.cpp",
                "LocalSchemeAlterColumnTypeService.h",
                "LocalSchemeCreateIndexService.cpp",
                "LocalSchemeCreateIndexService.h",
                "LocalSchemeCreateTableService.cpp",
                "LocalSchemeCreateTableService.h",
                "LocalSchemeDropColumnService.cpp",
                "LocalSchemeDropColumnService.h",
                "LocalSchemeDropIndexService.cpp",
                "LocalSchemeDropIndexService.h",
                "LocalSchemeDropTableService.cpp",
                "LocalSchemeDropTableService.h",
                "LocalSchemeRenameColumnService.cpp",
                "LocalSchemeRenameColumnService.h",
                "LocalSchemeRenameTableService.cpp",
                "LocalSchemeRenameTableService.h",
            ]
        }
    }

    Group {
        name: "MigrationExecution"
        prefix: "MigrationExecution/"

        files: [
            "LocalSchemeMigrationExecutionContext.cpp",
            "LocalSchemeMigrationExecutionContext.h",
            "LocalSchemeMigrationExecutionService.cpp",
            "LocalSchemeMigrationExecutionService.h",
            "MigrationExecutionConfig.cpp",
            "MigrationExecutionConfig.h",
            "MigrationExecutionContext.cpp",
            "MigrationExecutionContext.h",
            "MigrationExecutionService.cpp",
            "MigrationExecutionService.h",
        ]
    }

    Group {
        name: "Migrations"
        prefix: "Migrations/"

        files: [
            "Migration.cpp",
            "Migration.h",
            "MigrationRepository.cpp",
            "MigrationRepository.h",
            "RegisterMigration.h",
        ]
    }

    Group {
        name: "MigrationTracker"
        prefix: "MigrationTracker/"

        files: [
            "MigrationTrackerService.h",
        ]
    }

    Group {
        name: "QSqlMigrator"
        prefix: "QSqlMigrator/"

        files: [
            "QSqlMigratorConfig.cpp",
            "QSqlMigratorConfig.h",
            "QSqlMigratorService.cpp",
            "QSqlMigratorService.h",
        ]
    }

    Group {
        name: "Scheme"
        prefix: "Scheme/"

        files: [
            "Database.cpp",
            "Database.h",
            "DbColumn.cpp",
            "DbColumn.h",
            "DbTable.cpp",
            "DbTable.h",
        ]
    }

    Group {
        name: "Structure"
        prefix: "Structure/"

        files: [
            "Column.cpp",
            "Column.h",
            "Index.cpp",
            "Index.h",
            "LocalScheme.cpp",
            "LocalScheme.h",
            "Table.cpp",
            "Table.h",
            "Type.cpp",
            "Type.h",
        ]
    }
}


