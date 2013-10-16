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
win32: LIBS += -L$$LIB_PATH
else:unix: LIBS += -L$$LIB_PATH -Wl,-rpath,$$LIB_PATH

DESTDIR = $$QSQLMIGRATOR_ROOT/bin
CONFIG(release, debug|release): TARGET = tst_PostgresqlTest
else:CONFIG(debug, debug|release): TARGET = tst_PostgresqlTestd

win32:QMAKE_POST_LINK = $$DESTDIR/$${TARGET}.exe
else:unix:QMAKE_POST_LINK = $$DESTDIR/$${TARGET}

!exists(PostgresqlConfig.h) {
  system($$QMAKE_COPY PostgresqlConfig.h.example PostgresqlConfig.h)
}

# depends QSqlMigrator.lib {
CONFIG(release, debug|release): LIBS += -lQSqlMigrator
else:CONFIG(debug, debug|release): LIBS += -lQSqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigrator.dll
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigratord.dll
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libQSqlMigrator.so
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libQSqlMigratord.so
# }

# depends PostgresqlMigrator.lib {
CONFIG(release, debug|release): LIBS += -lPostgresqlMigrator
else:CONFIG(debug, debug|release): LIBS += -lPostgresqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/PostgresqlMigrator.dll
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/PostgresqlMigratord.dll
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libPostgresqlMigrator.so
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libPostgresqlMigratord.so
# }

SOURCES += tst_PostgresqlTest.cpp \
	 ../BasicTest/BasicTest.cpp

HEADERS += \
	 ../BasicTest/BasicTest.h \
         PostgresqlConfig.h
