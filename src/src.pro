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

QSQLMIGRATOR_ROOT = $$quote($$PWD)/..
LIB_PATH = $$QSQLMIGRATOR_ROOT/bin
DESTDIR = $$LIB_PATH

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src

win32:CONFIG(release, debug|release): TARGET = QSqlMigrator
else:win32:CONFIG(debug, debug|release): TARGET = QSqlMigratord

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
    Scheme/DbTable.cpp

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
    Scheme/DbTable.h
