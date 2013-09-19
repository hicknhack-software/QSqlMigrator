#-------------------------------------------------
#
# Project created by QtCreator 2013-01-25T13:11:08
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
CONFIG(release, debug|release): TARGET = tst_SqliteTest
else:CONFIG(debug, debug|release): TARGET = tst_SqliteTestd

win32:QMAKE_POST_LINK = $$DESTDIR/$${TARGET}.exe
else:unix:QMAKE_POST_LINK = $$DESTDIR/$${TARGET}

!exists(SqliteConfig.h) {
  system($$QMAKE_COPY SqliteConfig.h.example SqliteConfig.h)
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

# depends SqliteMigrator.lib {
CONFIG(release, debug|release): LIBS += -lSqliteMigrator
else:CONFIG(debug, debug|release): LIBS += -lSqliteMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigrator.dll
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigratord.dll
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigrator.so
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigratord.so
# }

SOURCES += tst_SqliteTest.cpp \
	 ../BasicTest/BasicTest.cpp

HEADERS += \
	 ../BasicTest/BasicTest.h \
	 SqliteConfig.h
