import qbs.File

Application {
    id: product
    name: "Example"
    consoleApplication: true

    Depends { name: "SqliteMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    Probe {
        id: config
        configure: {
            if (!File.exists(product.sourceDirectory + "/Config.h")) {
                File.copy(product.sourceDirectory + "/Config.h.example", product.sourceDirectory + "/Config.h")
            }
        }
    }

    files: [
        "Config.h",
        "Config.h.example",
        "example.cpp",
        "MyAppMigrator.cpp",
        "MyAppMigrator.h",
        "M201503301340654_CreateUsers.h",
        "M201503301340654_CreateUsers.cpp",
    ]
}
