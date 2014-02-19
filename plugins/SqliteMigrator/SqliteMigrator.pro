PLUGIN_TARGET=SqliteMigrator
include(../plugin.pri)

DEFINES += _BUILDING_SQLITEMIGRATOR_DLL

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
