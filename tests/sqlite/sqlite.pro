
TEST_NAME=Sqlite
include(../test.pri)

DEFINES += SRCDIR=\\\"$$PWD/\\\"

!exists(SqliteConfig.h) {
  system($$QMAKE_COPY SqliteConfig.h.example SqliteConfig.h)
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

SOURCES += tst_SqliteTest.cpp \
         ../BasicTest/BasicTest.cpp

HEADERS += \
         ../BasicTest/BasicTest.h \
         SqliteConfig.h
