import qbs.File

Application {
    id: product
    name: "TestMysql"
    consoleApplication: true
    type: ["application", "autotest"]
    install: true
    cpp.rpaths: ["../lib/"];
    cpp.cxxLanguageVersion: "c++17"

    Depends { name: "MysqlMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    Probe {
        id: config
        configure: {
            if (!File.exists(product.sourceDirectory + "/MysqlConfig.h")) {
                File.copy(product.sourceDirectory + "/MysqlConfig.h.example", product.sourceDirectory + "/MysqlConfig.h")
            }
        }
    }

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "MysqlConfig.h",
        "MysqlConfig.h.example",
        "tst_MysqlTest.cpp",
    ]
 }
