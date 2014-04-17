
TEMPLATE = app
QT       += testlib
CONFIG   += testcase
CONFIG   -= app_bundle

include(../common.pri)

win32: LIBS += -L$$LIB_PATH
else:unix: LIBS += -L$$LIB_PATH -Wl,-rpath,$$LIB_PATH

CONFIG(release, debug|release): TARGET = tst_$${TEST_NAME}Test
else:CONFIG(debug, debug|release): TARGET = tst_$${TEST_NAME}Testd

# depends QSqlMigrator.lib {
CONFIG(release, debug|release): LIBS += -lQSqlMigrator
else:CONFIG(debug, debug|release): LIBS += -lQSqlMigratord

DEPENDPATH += $$DESTDIR

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigrator.dll
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/QSqlMigratord.dll
else:unix:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libQSqlMigrator.so
else:unix:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/libQSqlMigratord.so
# }

# xunittest target {
xunittest.files =
xunittest.path = .

# If the test ends up in a different directory, we should cd to that directory.
XUNITTESTRUN_DESTDIR = $$DESTDIR
XUNITTESTRUN_TARGET= $$TARGET

win32:debug_and_release {
    contains(DESTDIR,^release$)|contains(DESTDIR,^debug$):XUNITTESTRUN_DESTDIR=

    XUNITTESTRUN_TARGET=$$replace(XUNITTESTRUN_TARGET,^\\.\\./,)
}
!isEmpty(XUNITTESTRUN_DESTDIR):!contains(XUNITTESTRUN_DESTDIR,^\\./?):xunittest.commands = cd $(DESTDIR) &&
contains(XUNITTESTRUN_TARGET,.*/.*):xunittest.commands = cd $(DESTDIR) &&

unix {
    mac:app_bundle: xunittest.commands += ./$(QMAKE_TARGET).app/Contents/MacOS/$(QMAKE_TARGET)
    else: xunittest.commands += ./$(QMAKE_TARGET)
} else {
    # Windows
    xunittest.commands += $(TARGET)
}

# Allow for custom arguments to tests
XUNITXML_PATH = $${DESTDIR}$${QMAKE_DIR_SEP}$(QMAKE_TARGET)-xunit.xml
win32: XUNITXML_PATH = $$replace(XUNITXML_PATH,/,\\)
xunittest.commands += -xml -o $${XUNITXML_PATH}

contains(QT_MAJOR_VERSION, 5): qtAddTargetEnv(xunittest.commands, QT)

# If the test is marked as insignificant, discard the exit code
insignificant_test:xunittest.commands = -$${xunittest.commands}

QMAKE_EXTRA_TARGETS *= xunittest
!debug_and_release|build_pass {
    xunittest.depends = first
} else {
    xunittest.CONFIG = recursive
    # In debug and release mode, only run the test once.
    # Run debug if that is the preferred config, release otherwise.
    debug_and_release {
        xunittest.target = dummy_xunittest
        xunittest.recurse_target = xunittest
        CONFIG(debug, debug|release) {
            real_xunittest.depends = debug-xunittest
            real_xunittest.target = xunittest
            QMAKE_EXTRA_TARGETS += real_xunittest
        } else {
            real_xunittest.depends = release-xunittest
            real_xunittest.target = xunittest
            QMAKE_EXTRA_TARGETS += real_xunittest
        }
    }
}

#}
