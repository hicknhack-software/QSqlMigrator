DynamicLibrary {
    name: "SqliteMigrator"

    Depends { name: "QSqlMigrator" }
    Depends { name: "cpp" }

    cpp.includePaths: ["../"]
    cpp.defines: ["_BUILDING_SQLITEMIGRATOR_DLL"]
    install: true

    Export {
        Depends { name: "QSqlMigrator" }
        Depends { name: "cpp" }
        cpp.includePaths: ["../"]
    }

    files: [
        "CommandExecution/SqliteAlterColumnService.cpp",
        "CommandExecution/SqliteAlterColumnService.h",
        "CommandExecution/SqliteAlterColumnTypeService.cpp",
        "CommandExecution/SqliteAlterColumnTypeService.h",
        "CommandExecution/SqliteDropColumnService.cpp",
        "CommandExecution/SqliteDropColumnService.h",
        "CommandExecution/SqliteRenameColumnService.cpp",
        "CommandExecution/SqliteRenameColumnService.h",
        "DatabaseLock.cpp",
        "DatabaseLock.h",
        "Helper/SqliteSqlStructureService.cpp",
        "Helper/SqliteSqlStructureService.h",
        "MigrationTracker/SqliteMigrationTableService.cpp",
        "MigrationTracker/SqliteMigrationTableService.h",
        "SqliteMigrator.cpp",
        "SqliteMigrator.h",
    ]
}

