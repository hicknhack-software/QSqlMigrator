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
LIBS += -L$$LIB_PATH

DESTDIR = $$QSQLMIGRATOR_ROOT/bin
win32:CONFIG(release, debug|release): TARGET = tst_SqliteTest
else:win32:CONFIG(debug, debug|release): TARGET = tst_SqliteTestd

QMAKE_POST_LINK = $$DESTDIR/$${TARGET}.exe

!exists(SqliteConfig.h) {
  system($$QMAKE_COPY SqliteConfig.h.example SqliteConfig.h)
}

# depends QSqlMigrator.lib {
win32:CONFIG(release, debug|release): LIBS += -lQSqlMigrator
else:win32:CONFIG(debug, debug|release): LIBS += -lQSqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigrator.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigratord.lib
# }

# depends SqliteMigrator.lib {
win32:CONFIG(release, debug|release): LIBS += -lSqliteMigrator
else:win32:CONFIG(debug, debug|release): LIBS += -lSqliteMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigrator.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigratord.lib
# }

SOURCES += tst_SqliteTest.cpp \
	 ../BasicTest/BasicTest.cpp

HEADERS += \
	 ../BasicTest/BasicTest.h \
	 SqliteConfig.h
