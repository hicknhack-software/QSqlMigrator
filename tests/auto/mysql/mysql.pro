
TEST_NAME=Mysql
include(../auto.pri)

DEFINES += SRCDIR=\\\"$$PWD/\\\"

!exists(MysqlConfig.h) {
  system($$QMAKE_COPY MysqlConfig.h.example MysqlConfig.h)
}

# depends MysqlMigrator.lib {
CONFIG(release, debug|release): LIBS += -lMysqlMigrator
else:CONFIG(debug, debug|release): LIBS += -lMysqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/MysqlMigrator.dll
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/MysqlMigratord.dll
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libMysqlMigrator.so
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libMysqlMigratord.so
# }

SOURCES += tst_MysqlTest.cpp \
	 ../BasicTest/BasicTest.cpp

HEADERS += \
	 ../BasicTest/BasicTest.h \
	 MysqlConfig.h

OTHER_FILES += \
	 MysqlDriver.txt
