Application {
    name: "mysqlTest"
    type: base.concat("autotest")

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
