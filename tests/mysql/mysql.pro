
TEST_NAME=Mysql
include(../test.pri)

DEFINES += SRCDIR=\\\"$$PWD/\\\"

!exists(MysqlConfig.h) {
  system($$QMAKE_COPY MysqlConfig.h.example MysqlConfig.h)
}

# depends MysqlMigrator.lib {
CONFIG(release, debug|release): LIBS += -lMysqlMigrator -lQSqlMigrator
else:CONFIG(debug, debug|release): LIBS += -lMysqlMigratord -lQSqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins
DEPENDPATH += $$DESTDIR

static {
    win32-msvc*:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/MysqlMigrator.lib
    else:win32-msvc*:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/MysqlMigratord.lib
    else:win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libMysqlMigrator.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libMysqlMigratord.a
    else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libMysqlMigrator.a
    else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libMysqlMigratord.a
} else {
    win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/MysqlMigrator.dll
    else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/MysqlMigratord.dll
    else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libMysqlMigrator.so
    else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libMysqlMigratord.so
}

SOURCES += tst_MysqlTest.cpp \
    ../BasicTest/BasicTest.cpp

HEADERS += \
    ../BasicTest/BasicTest.h \
    MysqlConfig.h \
    MysqlConfig.h.example

OTHER_FILES += \
    MysqlDriver.txt \
    MysqlConfig.h.appveyor \
    MysqlConfig.h.travis
