#-------------------------------------------------
#
# Project created by QtCreator 2013-01-25T13:44:25
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TEMPLATE = lib
CONFIG += dll
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

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigrator.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigratord.lib
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libQSqlMigrator.so
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libQSqlMigratord.so
# }

SOURCES += \
	 CommandExecution/SqliteRenameTableService.cpp \
	 CommandExecution/SqliteRenameColumnService.cpp \
	 CommandExecution/SqliteDropTableService.cpp \
	 CommandExecution/SqliteDropIndexService.cpp \
	 CommandExecution/SqliteDropColumnService.cpp \
	 CommandExecution/SqliteCreateTableService.cpp \
	 CommandExecution/SqliteCreateIndexService.cpp \
	 CommandExecution/SqliteAlterColumnTypeService.cpp \
	 CommandExecution/SqliteAlterColumnService.cpp \
	 CommandExecution/SqliteAddColumnService.cpp \
	 Helper/SqliteDbReader.cpp \
	 Helper/SqliteColumnService.cpp \
	 MigrationTracker/SqliteMigrationTableService.cpp \
	 SqliteMigrator.cpp \
    DatabaseLock.cpp

HEADERS += \
	 CommandExecution/SqliteRenameTableService.h \
	 CommandExecution/SqliteRenameColumnService.h \
	 CommandExecution/SqliteDropTableService.h \
	 CommandExecution/SqliteDropIndexService.h \
	 CommandExecution/SqliteDropColumnService.h \
	 CommandExecution/SqliteCreateTableService.h \
	 CommandExecution/SqliteCreateIndexService.h \
	 CommandExecution/SqliteAlterColumnTypeService.h \
	 CommandExecution/SqliteAlterColumnService.h \
	 CommandExecution/SqliteAddColumnService.h \
	 Helper/SqliteDbReader.h \
	 Helper/SqliteColumnService.h \
	 MigrationTracker/SqliteMigrationTableService.h \
	 SqliteMigrator.h \
    DatabaseLock.h

unix:!symbian {
	 maemo5 {
		  target.path = /opt/usr/lib
	 } else {
		  target.path = /usr/lib
	 }
	 INSTALLS += target
}
