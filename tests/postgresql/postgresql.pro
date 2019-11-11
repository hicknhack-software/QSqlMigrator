
TEST_NAME=Postgresql
include(../test.pri)

DEFINES += SRCDIR=\\\"$$PWD/\\\"

!exists(PostgresqlConfig.h) {
  system($$QMAKE_COPY PostgresqlConfig.h.example PostgresqlConfig.h)
}

# depends PostgresqlMigrator.lib {
CONFIG(release, debug|release): LIBS += -lPostgresqlMigrator -lQSqlMigrator
else:CONFIG(debug, debug|release): LIBS += -lPostgresqlMigratord -lQSqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins
DEPENDPATH += $$DESTDIR

static {
    win32-msvc*:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/PostgresqlMigrator.lib
    else:win32-msvc*:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/PostgresqlMigratord.lib
    else:win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libPostgresqlMigrator.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libPostgresqlMigratord.a
    else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libPostgresqlMigrator.a
    else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libPostgresqlMigratord.a
} else {
    win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/PostgresqlMigrator.dll
    else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/PostgresqlMigratord.dll
    else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libPostgresqlMigrator.so
    else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libPostgresqlMigratord.so
}

SOURCES += tst_PostgresqlTest.cpp \
    ../BasicTest/BasicTest.cpp

HEADERS += \
    ../BasicTest/BasicTest.h \
    PostgresqlConfig.h \
    PostgresqlConfig.h.example

OTHER_FILES += \
    PostgresqlDriver.txt \
    PostgresqlConfig.h.appveyor \
    PostgresqlConfig.h.travis
