
QT       += core sql
QT       -= gui

CONFIG += debug_and_release c++11

!win32 {
    equals(QT_MAJOR_VERSION, 4): QMAKE_CXXFLAGS += -std=c++0x
}

QSQLMIGRATOR_ROOT = $$quote($$PWD)

LIB_PATH = $$QSQLMIGRATOR_ROOT/bin
DESTDIR = $$LIB_PATH

INCLUDEPATH += $$QSQLMIGRATOR_ROOT/src

!contains(QMAKE_EXTRA_TARGETS, xunittest) {
    xunittest.depends = first
    QMAKE_EXTRA_TARGETS += xunittest
}
