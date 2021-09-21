import qbs.File

Application {
    id: product
    name: "TestPostgresql"
    consoleApplication: true
    type: ["application", "autotest"]
    install: true
    cpp.rpaths: ["../lib/"];

    Depends { name: "PostgresqlMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    Probe {
        id: config
        configure: {
            if (!File.exists(product.sourceDirectory + "/PostgresqlConfig.h")) {
                File.copy(product.sourceDirectory + "/PostgresqlConfig.h.example", product.sourceDirectory + "/PostgresqlConfig.h")
            }
        }
    }

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "PostgresqlConfig.h",
        "PostgresqlConfig.h.example",
        "tst_PostgresqlTest.cpp",
    ]
}
