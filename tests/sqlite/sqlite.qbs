Application {
    name: "sqliteTest"
    consoleApplication: true
    type: ["application", "autotest"]
    install: true

    Depends { name: "SqliteMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    cpp.rpaths: ["../lib/"];

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "SqliteConfig.h",
        "SqliteConfig.h.example",
        "tst_SqliteTest.cpp",
    ]
}
