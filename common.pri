
QT       += core sql
QT       -= gui

CONFIG += debug_and_release c++11

QSQLMIGRATOR_ROOT = $$quote($$PWD)

LIB_PATH = $$QSQLMIGRATOR_ROOT/bin
DESTDIR = $$LIB_PATH

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src
