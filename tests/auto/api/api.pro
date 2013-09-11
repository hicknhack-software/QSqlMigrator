#-------------------------------------------------
#
# Project created by QtCreator 2013-01-25T17:07:12
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
CONFIG(release, debug|release): TARGET = tst_ApiTest
else:CONFIG(debug, debug|release): TARGET = tst_ApiTestd


win32:QMAKE_POST_LINK = $$DESTDIR/$${TARGET}.exe
else:unix:QMAKE_POST_LINK = $$DESTDIR/$${TARGET}

!exists(ApiConfig.h) {
  system($$QMAKE_COPY ApiConfig.h.example ApiConfig.h)
}

# depends QSqlMigrator.lib {
CONFIG(release, debug|release): LIBS += -lQSqlMigrator
else:CONFIG(debug, debug|release): LIBS += -lQSqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigrator.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigratord.lib
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libQSqlMigrator.so
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libQSqlMigratord.so
# }

# depends SqliteMigrator.lib {
CONFIG(release, debug|release): LIBS += -lSqliteMigrator
else:CONFIG(debug, debug|release): LIBS += -lSqliteMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigrator.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/SqliteMigratord.lib
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigrator.so
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libSqliteMigratord.so
# }

SOURCES += tst_ApiTest.cpp \
	 M20132201_180943_CreateCars.cpp \
	 M20132201_175827_CreateAddresses.cpp \
	 M20131501_191807_CreateUsers.cpp \
	 M20133001_164323_AddUsers.cpp \
	 SqliteAddUser.cpp

HEADERS += \
	 M20132201_180943_CreateCars.h \
	 M20132201_175827_CreateAddresses.h \
	 M20131501_191807_CreateUsers.h \
	 M20133001_164323_AddUsers.h \
	 SqliteAddUser.h \
	 ApiConfig.h \
	 ApiConfig.h.example
