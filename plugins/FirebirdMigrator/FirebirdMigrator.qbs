DynamicLibrary {
    name: "FirebirdMigrator"

    Depends { name: "Qt"; submodules: ["core", "sql"] }
    Depends { name: "QSqlMigrator" }

    cpp.defines: ["_BUILDING_FIREBIRDMIGRATOR_DLL"]
    cpp.includePaths: ["../"]

    Export {
        Depends { name: "cpp" }
        Depends { name: "QSqlMigrator" }
        cpp.dynamicLibraries: {"QSqlMigrator"}
        cpp.includePaths: ["../"]
    }
    files: [
        "FirebirdMigrator.cpp",
        "FirebirdMigrator.h",
        "Helper/FirebirdSqlStructureService.cpp",
        "Helper/FirebirdSqlStructureService.h"
    ]
}

