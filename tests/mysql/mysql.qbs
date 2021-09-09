Application {
    name: "mysqlTest"
    consoleApplication: true
    type: ["application", "autotest"]
    install: true

    Depends { name: "MysqlMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    cpp.rpaths: ["../lib/"];
    Properties {
        condition: qbs.toolchain.contains("clang")
        cpp.commonCompilerFlags: "-Wno-deprecated-copy"
    }

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "MysqlConfig.h",
        "MysqlConfig.h.example",
        "tst_MysqlTest.cpp",
    ]
 }
