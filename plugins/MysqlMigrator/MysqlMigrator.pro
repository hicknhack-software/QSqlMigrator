#-------------------------------------------------
#
# Project created by QtCreator 2013-01-30T15:10:08
#
#-------------------------------------------------

QT       += sql
QT       -= gui

TEMPLATE = lib
CONFIG += dll debug_and_release
DEFINES += _BUILDING_MYSQLMIGRATOR_DLL

QSQLMIGRATOR_ROOT = $$quote($$PWD)/../..

LIB_PATH = $$QSQLMIGRATOR_ROOT/bin
DESTDIR = $$LIB_PATH
win32: LIBS += -L$$LIB_PATH
unix: LIBS += -L$$LIB_PATH -Wl,-rpath,$$LIB_PATH

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/plugins

CONFIG(release, debug|release): TARGET = MysqlMigrator
else:CONFIG(debug, debug|release): TARGET = MysqlMigratord

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
     MysqlMigrator.cpp \
     Helper/MysqlDbReaderService.cpp \
     Helper/MysqlColumnService.cpp \
     Helper/MysqlTypeMapperService.cpp \
     CommandExecution/MysqlAlterColumnTypeService.cpp \
     CommandExecution/MysqlRenameColumnService.cpp

HEADERS += \
     MysqlMigrator.h \
     Helper/MysqlDbReaderService.h \
     Helper/MysqlColumnService.h \
     Helper/MysqlTypeMapperService.h \
     CommandExecution/MysqlAlterColumnTypeService.h \
     CommandExecution/MysqlRenameColumnService.h

unix:!symbian {
	 maemo5 {
		  target.path = /opt/usr/lib
	 } else {
		  target.path = /usr/lib
	 }
	 INSTALLS += target
}
