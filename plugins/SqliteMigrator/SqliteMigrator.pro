#-------------------------------------------------
#
# Project created by QtCreator 2013-01-25T13:44:25
#
#-------------------------------------------------

QT       += sql
QT       -= gui

TEMPLATE = lib
CONFIG += dll debug_and_release
DEFINES += _BUILDING_SQLITEMIGRATOR_DLL

QSQLMIGRATOR_ROOT = $$quote($$PWD)/../..

LIB_PATH = $$QSQLMIGRATOR_ROOT/bin
DESTDIR = $$LIB_PATH
win32: LIBS += -L$$LIB_PATH
else:unix: LIBS += -L$$LIB_PATH -Wl,-rpath,$$LIB_PATH

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins

CONFIG(release, debug|release): TARGET = SqliteMigrator
else:CONFIG(debug, debug|release): TARGET = SqliteMigratord

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

SOURCES += \
         DatabaseLock.cpp \
	 CommandExecution/SqliteDropColumnService.cpp \
	 CommandExecution/SqliteAlterColumnTypeService.cpp \
	 CommandExecution/SqliteAlterColumnService.cpp \
     CommandExecution/SqliteRenameColumnService.cpp \
         MigrationTracker/SqliteMigrationTableService.cpp \
         SqliteMigrator.cpp \
    Helper/SqliteSqlStructureService.cpp

HEADERS += \
         DatabaseLock.h \
	 CommandExecution/SqliteDropColumnService.h \
	 CommandExecution/SqliteAlterColumnTypeService.h \
	 CommandExecution/SqliteAlterColumnService.h \
     CommandExecution/SqliteRenameColumnService.h \
         MigrationTracker/SqliteMigrationTableService.h \
         SqliteMigrator.h \
    Helper/SqliteSqlStructureService.h

unix:!symbian {
	 maemo5 {
		  target.path = /opt/usr/lib
	 } else {
		  target.path = /usr/lib
	 }
	 INSTALLS += target
}
