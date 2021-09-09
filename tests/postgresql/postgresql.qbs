Application {
    name: "postgresqlTest"
    consoleApplication: true
    type: ["application", "autotest"]
    install: true

    Properties {
        condition: qbs.toolchain.contains("clang")
        cpp.commonCompilerFlags: "-Wno-deprecated-copy"
    }
    Depends { name: "PostgresqlMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    cpp.rpaths: ["../lib/"];

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "PostgresqlConfig.h",
        "PostgresqlConfig.h.example",
        "tst_PostgresqlTest.cpp",
    ]
}
