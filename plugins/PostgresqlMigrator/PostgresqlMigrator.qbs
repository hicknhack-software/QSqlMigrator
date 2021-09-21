DynamicLibrary {
    name: "PostgresqlMigrator"

    Depends { name: "QSqlMigrator" }
    Depends { name: "cpp" }

    cpp.includePaths: ["../"]
    cpp.defines: ["_BUILDING_POSTGRESQLMIGRATOR_DLL"]
    install: true

    Export {
        Depends { name: "QSqlMigrator" }
        Depends { name: "cpp" }
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

