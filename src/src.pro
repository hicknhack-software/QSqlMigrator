#-------------------------------------------------
#
# Project created by QtCreator 2013-01-25T11:54:21
#
#-------------------------------------------------

QT       += core sql
QT       -= gui

TEMPLATE = lib
CONFIG += dll debug_and_release
DEFINES += _BUILDING_QSQLMIGRATOR_DLL
DEFINES += LOG_PREFIX=\\\"[QSqlMigrator]\\\"

QSQLMIGRATOR_ROOT = $$quote($$PWD)/../
LIB_PATH = $$QSQLMIGRATOR_ROOT/bin
DESTDIR = $$LIB_PATH

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src

CONFIG(release, debug|release): TARGET = QSqlMigrator
else:CONFIG(debug, debug|release): TARGET = QSqlMigratord

SOURCES += \
	 QSqlMigrator/QSqlMigratorService.cpp \
	 QSqlMigrator/QSqlMigratorConfig.cpp \
	 CommandExecution/CommandExecutionServiceRepository.cpp \
	 CommandExecution/CommandExecutionService.cpp \
	 CommandExecution/CommandExecutionContext.cpp \
	 CommandExecution/BaseCommandExecutionService.cpp \
     CommandExecution/LocalSchemeBaseCommandExecutionService.cpp \
     CommandExecution/LocalSchemeCommandExecutionContext.cpp \
     CommandExecution/LocalSchemeCommandExecutionService.cpp \
     CommandExecution/LocalSchemeCommandExecutionServiceRepository.cpp \
	 Commands/RenameTable.cpp \
	 Commands/RenameColumn.cpp \
	 Commands/DropTable.cpp \
	 Commands/DropIndex.cpp \
	 Commands/DropColumn.cpp \
	 Commands/CreateTable.cpp \
	 Commands/CreateIndex.cpp \
	 Commands/BaseCommand.cpp \
	 Commands/AlterColumnType.cpp \
	 Commands/AddColumn.cpp \
	 Migrations/MigrationRepository.cpp \
	 Migrations/Migration.cpp \
	 MigrationExecution/MigrationExecutionService.cpp \
	 MigrationExecution/MigrationExecutionContext.cpp \
	 MigrationExecution/MigrationExecutionConfig.cpp \
     MigrationExecution/LocalSchemeMigrationExecutionService.cpp \
     MigrationExecution/LocalSchemeMigrationExecutionContext.cpp \
     Structure/Table.cpp \
	 Structure/Index.cpp \
	 Structure/Column.cpp \
	 CommandExecution/CustomCommandService.cpp \
	 Commands/CustomCommandBase.cpp \
    Scheme/Database.cpp \
    Scheme/DbColumn.cpp \
    Scheme/DbTable.cpp \
    BaseSqlMigrator/CommandExecution/BaseSqlAddColumnService.cpp \
    BaseSqlMigrator/CommandExecution/BaseSqlAlterColumnTypeService.cpp \
    BaseSqlMigrator/CommandExecution/BaseSqlCreateIndexService.cpp \
    BaseSqlMigrator/CommandExecution/BaseSqlCreateTableService.cpp \
    BaseSqlMigrator/CommandExecution/BaseSqlDropColumnService.cpp \
    BaseSqlMigrator/CommandExecution/BaseSqlDropIndexService.cpp \
    BaseSqlMigrator/CommandExecution/BaseSqlDropTableService.cpp \
    BaseSqlMigrator/CommandExecution/BaseSqlRenameColumnService.cpp \
    BaseSqlMigrator/CommandExecution/BaseSqlRenameTableService.cpp \
    BaseSqlMigrator/Helper/BaseSqlColumnService.cpp \
    BaseSqlMigrator/Helper/BaseSqlQuoteService.cpp \
    BaseSqlMigrator/Helper/BaseSqlTypeMapperService.cpp \
    BaseSqlMigrator/MigrationTracker/MigrationTableService.cpp \
    Helper/HelperAggregate.cpp \
    Helper/ColumnService.cpp \
    Helper/DbReaderService.cpp \
    Helper/QuoteService.cpp \
    Helper/TypeMapperService.cpp \
    Structure/SqlType.cpp \
    Structure/LocalScheme.cpp \
    LocalSchemeMigrator/LocalSchemeMigrator.cpp \
    LocalSchemeMigrator/CommandExecution/LocalSchemeAddColumnService.cpp \
    LocalSchemeMigrator/CommandExecution/LocalSchemeAlterColumnTypeService.cpp \
    LocalSchemeMigrator/CommandExecution/LocalSchemeCreateIndexService.cpp \
    LocalSchemeMigrator/CommandExecution/LocalSchemeCreateTableService.cpp \
    LocalSchemeMigrator/CommandExecution/LocalSchemeDropColumnService.cpp \
    LocalSchemeMigrator/CommandExecution/LocalSchemeDropIndexService.cpp \
    LocalSchemeMigrator/CommandExecution/LocalSchemeDropTableService.cpp \
    LocalSchemeMigrator/CommandExecution/LocalSchemeRenameColumnService.cpp \
    LocalSchemeMigrator/LocalSchemeComparisonService.cpp \
    LocalSchemeMigrator/LocalSchemeComparisonContext.cpp

HEADERS += \
	 QSqlMigrator/QSqlMigratorService.h \
	 QSqlMigrator/QSqlMigratorConfig.h \
	 CommandExecution/CommandExecutionServiceRepository.h \
	 CommandExecution/CommandExecutionService.h \
	 CommandExecution/CommandExecutionContext.h \
	 CommandExecution/BaseCommandExecutionService.h \
     CommandExecution/LocalSchemeBaseCommandExecutionService.h \
     CommandExecution/LocalSchemeCommandExecutionContext.h \
     CommandExecution/LocalSchemeCommandExecutionService.h \
     CommandExecution/LocalSchemeCommandExecutionServiceRepository.h \
     Commands/RenameTable.h \
	 Commands/RenameColumn.h \
	 Commands/DropTable.h \
	 Commands/DropIndex.h \
	 Commands/DropColumn.h \
	 Commands/CreateTable.h \
	 Commands/CreateIndex.h \
	 Commands/BaseCommand.h \
	 Commands/AlterColumnType.h \
	 Commands/AddColumn.h \
	 Migrations/RegisterMigration.h \
	 Migrations/MigrationRepository.h \
	 Migrations/Migration.h \
	 MigrationExecution/MigrationExecutionService.h \
	 MigrationExecution/MigrationExecutionContext.h \
	 MigrationExecution/MigrationExecutionConfig.h \
     MigrationExecution/LocalSchemeMigrationExecutionService.h \
     MigrationExecution/LocalSchemeMigrationExecutionContext.h \
     MigrationTracker/BaseMigrationTrackerService.h \
	 Structure/Table.h \
	 Structure/Index.h \
	 Structure/Column.h \
	 api.h \
	 CommandExecution/CustomCommandService.h \
	 Commands/CustomCommandBase.h \
	 config.h \
    Scheme/Database.h \
    Scheme/DbColumn.h \
    Scheme/DbTable.h \
    BaseSqlMigrator/CommandExecution/BaseSqlAddColumnService.h \
    BaseSqlMigrator/CommandExecution/BaseSqlAlterColumnTypeService.h \
    BaseSqlMigrator/CommandExecution/BaseSqlCreateIndexService.h \
    BaseSqlMigrator/CommandExecution/BaseSqlCreateTableService.h \
    BaseSqlMigrator/CommandExecution/BaseSqlDropColumnService.h \
    BaseSqlMigrator/CommandExecution/BaseSqlDropIndexService.h \
    BaseSqlMigrator/CommandExecution/BaseSqlDropTableService.h \
    BaseSqlMigrator/CommandExecution/BaseSqlRenameColumnService.h \
    BaseSqlMigrator/CommandExecution/BaseSqlRenameTableService.h \
    BaseSqlMigrator/Helper/BaseSqlColumnService.h \
    BaseSqlMigrator/Helper/BaseSqlQuoteService.h \
    BaseSqlMigrator/Helper/BaseSqlTypeMapperService.h \
    BaseSqlMigrator/MigrationTracker/MigrationTableService.h \
    Helper/HelperAggregate.h \
    Helper/ColumnService.h \
    Helper/DbReaderService.h \
    Helper/QuoteService.h \
    Helper/TypeMapperService.h \
    Structure/SqlType.h \
    Structure/LocalScheme.h \
    LocalSchemeMigrator/LocalSchemeMigrator.h \
    LocalSchemeMigrator/CommandExecution/LocalSchemeAddColumnService.h \
    LocalSchemeMigrator/CommandExecution/LocalSchemeAlterColumnTypeService.h \
    LocalSchemeMigrator/CommandExecution/LocalSchemeCreateIndexService.h \
    LocalSchemeMigrator/CommandExecution/LocalSchemeCreateTableService.h \
    LocalSchemeMigrator/CommandExecution/LocalSchemeDropColumnService.h \
    LocalSchemeMigrator/CommandExecution/LocalSchemeDropIndexService.h \
    LocalSchemeMigrator/CommandExecution/LocalSchemeDropTableService.h \
    LocalSchemeMigrator/CommandExecution/LocalSchemeRenameColumnService.h \
    LocalSchemeMigrator/LocalSchemeComparisonService.h \
    LocalSchemeMigrator/LocalSchemeComparisonContext.h
