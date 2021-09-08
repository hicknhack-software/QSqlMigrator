Application {
    name: "sqliteTest"
    type: base.concat("autotest")

    Depends { name: "SqliteMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "SqliteConfig.h",
        "SqliteConfig.h.example",
        "tst_SqliteTest.cpp",
    ]
}
