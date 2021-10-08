import qbs.File

Application {
    id: product
    name: "TestSqlite"
    consoleApplication: true
    type: ["application", "autotest"]
    install: true

    Depends { name: "SqliteMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    Probe {
        id: config
        configure: {
            if (!File.exists(product.sourceDirectory + "/SqliteConfig.h")) {
                File.copy(product.sourceDirectory + "/SqliteConfig.h.example", product.sourceDirectory + "/SqliteConfig.h")
            }
        }
    }
    cpp.rpaths: ["../lib/"]
    cpp.cxxLanguageVersion: "c++17"

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "SqliteConfig.h",
        "SqliteConfig.h.example",
        "tst_SqliteTest.cpp",
    ]
}
