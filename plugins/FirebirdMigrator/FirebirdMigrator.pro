PLUGIN_TARGET=FirebirdMigrator
include(../plugin.pri)

DEFINES += _BUILDING_FIREBIRDMIGRATOR_DLL

HEADERS += \
	 FirebirdMigrator.h \
    Helper/FirebirdSqlStructureService.h

SOURCES += \
	 FirebirdMigrator.cpp \
    Helper/FirebirdSqlStructureService.cpp

