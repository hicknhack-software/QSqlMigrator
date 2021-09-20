Application {
    name: "postgresqlTest"
    consoleApplication: true
    type: ["application", "autotest"]
    install: true
    cpp.rpaths: ["../lib/"];

    Depends { name: "PostgresqlMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "PostgresqlConfig.h",
        "PostgresqlConfig.h.example",
        "tst_PostgresqlTest.cpp",
    ]
}
