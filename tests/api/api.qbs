Application {
    name: "ApiTest"
    consoleApplication: true
    type: base.concat("autotest")

    Depends { name: "SqliteMigrator" }
    Depends { name: "Qt"; submodules: ["test"] }


    files: [
        "../BasicTest/BasicTest.cpp",
        "../BasicTest/BasicTest.h",
        "ApiConfig.h",
        "ApiConfig.h.example",
        "M20131501_191807_CreateUsers.cpp",
        "M20131501_191807_CreateUsers.h",
        "M20132201_175827_CreateAddresses.cpp",
        "M20132201_175827_CreateAddresses.h",
        "M20132201_180943_CreateCars.cpp",
        "M20132201_180943_CreateCars.h",
        "M20133001_164323_AddUsers.cpp",
        "M20133001_164323_AddUsers.h",
        "SqliteAddUser.cpp",
        "SqliteAddUser.h",
        "tst_ApiTest.cpp",
    ]
}
