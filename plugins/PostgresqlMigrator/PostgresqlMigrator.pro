PLUGIN_TARGET=PostgresqlMigrator
include(../plugin.pri)

DEFINES += _BUILDING_POSTGRESQLMIGRATOR_DLL

SOURCES += \
         PostgresqlMigrator.cpp \
         Helper/PostgresqlQuoteService.cpp \
         Helper/PostgresqlStructureService.cpp \
         Helper/PostgresqlColumnService.cpp \
         Helper/PostgresqlTypeMapperService.cpp

HEADERS += \
         PostgresqlMigrator.h \
         Helper/PostgresqlQuoteService.h \
         Helper/PostgresqlStructureService.h \
         Helper/PostgresqlColumnService.h \
         Helper/PostgresqlTypeMapperService.h
