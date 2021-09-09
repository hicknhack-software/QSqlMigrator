Application {
    name: "postgresqlTest"
    type: base.concat("autotest")

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
