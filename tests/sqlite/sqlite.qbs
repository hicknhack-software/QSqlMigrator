Application {
    name: "sqliteTest"
    consoleApplication: true
    type: ["application", "autotest"]
    install: true

    Depends { name: "SqliteMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }

    Properties {
        condition: qbs.toolchain.contains("clang")
        cpp.commonCompilerFlags: "-Wno-deprecated-copy"
    }
    cpp.rpaths: ["../lib/"];

    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "SqliteConfig.h",
        "SqliteConfig.h.example",
        "tst_SqliteTest.cpp",
    ]
}
