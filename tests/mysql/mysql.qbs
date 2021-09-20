Application {
    name: "mysqlTest"
    consoleApplication: true
    type: ["application", "autotest"]
    install: true
    cpp.rpaths: ["../lib/"];

    Depends { name: "MysqlMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "MysqlConfig.h",
        "MysqlConfig.h.example",
        "tst_MysqlTest.cpp",
    ]
 }
