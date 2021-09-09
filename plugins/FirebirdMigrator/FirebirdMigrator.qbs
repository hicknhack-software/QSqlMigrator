DynamicLibrary {
    name: "FirebirdMigrator"

    Depends { name: "QSqlMigrator" }

    Depends { name: "cpp" }
    cpp.includePaths: ["../"]
    cpp.defines: ["_BUILDING_FIREBIRDMIGRATOR_DLL"]
    install: true

    Properties {
        condition: qbs.toolchain.contains("clang")
        cpp.commonCompilerFlags: "-Wno-deprecated-copy"
    }

    Export {
        Depends { name: "QSqlMigrator" }
        Depends { name: "cpp" }
        cpp.includePaths: ["../"]
    }

    files: [
        "FirebirdMigrator.cpp",
        "FirebirdMigrator.h",
        "Helper/FirebirdSqlStructureService.cpp",
        "Helper/FirebirdSqlStructureService.h"
    ]
}

