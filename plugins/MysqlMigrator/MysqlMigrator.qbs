DynamicLibrary {
    name: "MysqlMigrator"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "sql"] }
    Depends { name: "QSqlMigrator" }
    cpp.includePaths: ["../"]
    cpp.defines: ["_BUILDING_MYSQLMIGRATOR_DLL"]

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core", "sql"] }
        Depends { name: "QSqlMigrator" }
        cpp.dynamicLibraries: {"QSqlMigrator"}
        cpp.includePaths: ["../"]
    }

    files: [
        "CommandExecution/MysqlAlterColumnTypeService.cpp",
        "CommandExecution/MysqlAlterColumnTypeService.h",
        "CommandExecution/MysqlRenameColumnService.cpp",
        "CommandExecution/MysqlRenameColumnService.h",
        "Helper/MysqlColumnService.cpp",
        "Helper/MysqlColumnService.h",
        "Helper/MysqlStructureService.cpp",
        "Helper/MysqlStructureService.h",
        "Helper/MysqlTypeMapperService.cpp",
        "Helper/MysqlTypeMapperService.h",
        "MysqlMigrator.cpp",
        "MysqlMigrator.h",
    ]
}
