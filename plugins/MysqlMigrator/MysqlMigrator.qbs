DynamicLibrary {
    name: "MysqlMigrator"

    Depends { name: "QSqlMigrator" }
    Depends { name: "cpp" }

    cpp.includePaths: ["../"]
    cpp.defines: ["_BUILDING_MYSQLMIGRATOR_DLL"]
    install: true

    Export {
        Depends { name: "QSqlMigrator" }
        Depends { name: "cpp" }
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
