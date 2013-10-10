#-------------------------------------------------
#
# Project created by QtCreator 2013-01-25T11:54:21
#
#-------------------------------------------------

QT       += core sql
QT       -= gui

TEMPLATE = lib
CONFIG += dll
DEFINES += _BUILDING_QSQLMIGRATOR_DLL
DEFINES += LOG_PREFIX=\\\"[QSqlMigrator]\\\"

QSQLMIGRATOR_ROOT = $$quote($$PWD)/../
LIB_PATH = $$QSQLMIGRATOR_ROOT/bin
DESTDIR = $$LIB_PATH

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src

#its the same target for each os
CONFIG(release, debug|release): TARGET = QSqlMigrator
else:CONFIG(debug, debug|release): TARGET = QSqlMigratord

SOURCES += \
	 QSqlMigrator/QSqlMigratorService.cpp \
	 QSqlMigrator/QSqlMigratorConfig.cpp \
	 CommandExecution/CommandExecutionServiceRepository.cpp \
	 CommandExecution/CommandExecutionService.cpp \
	 CommandExecution/CommandExecutionContext.cpp \
	 CommandExecution/BaseCommandExecutionService.cpp \
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
	 Structure/Table.cpp \
	 Structure/Index.cpp \
	 Structure/Column.cpp \
	 CommandExecution/CustomCommandService.cpp \
	 Commands/CustomCommandBase.cpp \
    Scheme/Database.cpp \
    Scheme/DbColumn.cpp \
    Scheme/DbTable.cpp \
    BaseSqlMigrator/BaseSqlMigrator.cpp \
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
    Helper/HelperAggregate.cpp \
    Helper/ColumnService.cpp \
    Helper/DbReaderService.cpp \
    Helper/QuoteService.cpp \
    BaseSqlMigrator/Helper/BaseSqlDbReaderService.cpp

HEADERS += \
	 QSqlMigrator/QSqlMigratorService.h \
	 QSqlMigrator/QSqlMigratorConfig.h \
	 CommandExecution/CommandExecutionServiceRepository.h \
	 CommandExecution/CommandExecutionService.h \
	 CommandExecution/CommandExecutionContext.h \
	 CommandExecution/BaseCommandExecutionService.h \
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
    BaseSqlMigrator/BaseSqlMigrator.h \
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
    Helper/HelperAggregate.h \
    Helper/ColumnService.h \
    Helper/DbReaderService.h \
    Helper/QuoteService.h \
    BaseSqlMigrator/Helper/BaseSqlDbReaderService.h
