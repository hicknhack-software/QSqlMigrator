
TEST_NAME=Example
include(../tests/test.pri)

DEFINES += SRCDIR=\\\"$$PWD/\\\"

!exists(Config.h) {
  system($$QMAKE_COPY Config.h.example Config.h)
}

# depends SqliteMigrator.lib {
CONFIG(release, debug|release): LIBS += -lSqliteMigrator -lQSqlMigrator
else:CONFIG(debug, debug|release): LIBS += -lSqliteMigratord -lQSqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins
DEPENDPATH += $$DESTDIR

static {
    win32-msvc*:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigrator.lib
    else:win32-msvc*:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigratord.lib
    else:win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigrator.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigratord.a
    else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigrator.a
    else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigratord.a
} else {
    win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigrator.dll
    else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigratord.dll
    else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigrator.so
    else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigratord.so
}

HEADERS += \
    Config.h \
    Config.h.example \
    MyAppMigrator.h \
    M201503301340654_CreateUsers.h

SOURCES += \
    example.cpp \
    MyAppMigrator.cpp \
    M201503301340654_CreateUsers.cpp
