
PLUGIN_TARGET=MysqlMigrator
include(../plugin.pri)

DEFINES += _BUILDING_MYSQLMIGRATOR_DLL

SOURCES += \
     MysqlMigrator.cpp \
     Helper/MysqlStructureService.cpp \
     Helper/MysqlColumnService.cpp \
     Helper/MysqlTypeMapperService.cpp \
     CommandExecution/MysqlAlterColumnTypeService.cpp \
     CommandExecution/MysqlRenameColumnService.cpp

HEADERS += \
     MysqlMigrator.h \
     Helper/MysqlStructureService.h \
     Helper/MysqlColumnService.h \
     Helper/MysqlTypeMapperService.h \
     CommandExecution/MysqlAlterColumnTypeService.h \
     CommandExecution/MysqlRenameColumnService.h
