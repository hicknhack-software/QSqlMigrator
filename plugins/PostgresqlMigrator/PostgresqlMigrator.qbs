DynamicLibrary {
    name: "PostgresqlMigrator"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "sql"] }
    Depends { name: "QSqlMigrator" }
    cpp.includePaths: ["../"]
    cpp.defines: ["_BUILDING_POSTGRESQLMIGRATOR_DLL"]

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core", "sql"] }
        Depends { name: "QSqlMigrator" }
        cpp.dynamicLibraries: {"QSqlMigrator"}
        cpp.includePaths: ["../"]
    }
    files: [
        "Helper/PostgresqlColumnService.cpp",
        "Helper/PostgresqlColumnService.h",
        "Helper/PostgresqlQuoteService.cpp",
        "Helper/PostgresqlQuoteService.h",
        "Helper/PostgresqlStructureService.cpp",
        "Helper/PostgresqlStructureService.h",
        "Helper/PostgresqlTypeMapperService.cpp",
        "Helper/PostgresqlTypeMapperService.h",
        "PostgresqlMigrator.cpp",
        "PostgresqlMigrator.h",
    ]
}

