#-------------------------------------------------
#
# Project created by QtCreator 2013-01-30T15:10:08
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TEMPLATE = lib
CONFIG += dll
DEFINES += _BUILDING_MYSQLMIGRATOR_DLL

QSQLMIGRATOR_ROOT = $$quote($$PWD)/../..

LIB_PATH = $$QSQLMIGRATOR_ROOT/bin
DESTDIR = $$LIB_PATH
LIBS += -L$$LIB_PATH

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins

win32:CONFIG(release, debug|release): TARGET = MysqlMigrator
else:win32:CONFIG(debug, debug|release): TARGET = MysqlMigratord

# depends QSqlMigrator.lib {
win32:CONFIG(release, debug|release): LIBS += -lQSqlMigrator
else:win32:CONFIG(debug, debug|release): LIBS += -lQSqlMigratord

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src
DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigrator.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigratord.lib
# }

SOURCES += \
	 CommandExecution/MysqlDropTableService.cpp \
	 CommandExecution/MysqlCreateTableService.cpp \
	 Helper/MysqlQuoteService.cpp \
	 Helper/MysqlColumnService.cpp \
	 MigrationTracker/MysqlMigrationTableService.cpp \
	 CommandExecution/MysqlAddColumnService.cpp \
	 CommandExecution/MysqlAlterColumnTypeService.cpp \
	 CommandExecution/MysqlCreateIndexService.cpp \
	 CommandExecution/MysqlDropColumnService.cpp \
	 CommandExecution/MysqlDropIndexService.cpp \
	 CommandExecution/MysqlRenameColumnService.cpp \
	 CommandExecution/MysqlRenameTableService.cpp \
	 Helper/MysqlDbReader.cpp \
	 MysqlMigrator.cpp

HEADERS += \
	 CommandExecution/MysqlDropTableService.h \
	 CommandExecution/MysqlCreateTableService.h \
	 Helper/MysqlQuoteService.h \
	 Helper/MysqlColumnService.h \
	 MigrationTracker/MysqlMigrationTableService.h \
	 CommandExecution/MysqlAddColumnService.h \
	 CommandExecution/MysqlAlterColumnTypeService.h \
	 CommandExecution/MysqlCreateIndexService.h \
	 CommandExecution/MysqlDropColumnService.h \
	 CommandExecution/MysqlDropIndexService.h \
	 CommandExecution/MysqlRenameColumnService.h \
	 CommandExecution/MysqlRenameTableService.h \
	 Helper/MysqlDbReader.h \
	 MysqlMigrator.h

unix:!symbian {
	 maemo5 {
		  target.path = /opt/usr/lib
	 } else {
		  target.path = /usr/lib
	 }
	 INSTALLS += target
}
