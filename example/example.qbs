Application {
    name: "Example"
    consoleApplication: true

    Depends { name: "SqliteMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

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
