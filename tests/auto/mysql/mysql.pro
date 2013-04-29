#-------------------------------------------------
#
# Project created by QtCreator 2013-01-30T18:03:25
#
#-------------------------------------------------

QT       += sql testlib
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DEFINES += SRCDIR=\\\"$$PWD/\\\"

QSQLMIGRATOR_ROOT = $$quote($$PWD)/../../../
LIB_PATH = $$QSQLMIGRATOR_ROOT/bin
LIBS += -L$$LIB_PATH

DESTDIR = $$QSQLMIGRATOR_ROOT/bin
win32:CONFIG(release, debug|release): TARGET = tst_MysqlTest
else:win32:CONFIG(debug, debug|release): TARGET = tst_MysqlTestd

QMAKE_POST_LINK = $$DESTDIR/$${TARGET}.exe

!exists(MysqlConfig.h) {
  system($$QMAKE_COPY MysqlConfig.h.example MysqlConfig.h)
}

# depends QSqlMigrator.lib {
win32:CONFIG(release, debug|release): LIBS += -lQSqlMigrator
else:win32:CONFIG(debug, debug|release): LIBS += -lQSqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigrator.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigratord.lib
# }

# depends MysqlMigrator.lib {
win32:CONFIG(release, debug|release): LIBS += -lMysqlMigrator
else:win32:CONFIG(debug, debug|release): LIBS += -lMysqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/MysqlMigrator.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/MysqlMigratord.lib
# }

SOURCES += tst_MysqlTest.cpp \
	 ../BasicTest/BasicTest.cpp

HEADERS += \
	 ../BasicTest/BasicTest.h \
	 MysqlConfig.h

OTHER_FILES += \
	 MysqlDriver.txt
