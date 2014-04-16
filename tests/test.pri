
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
XUNITTESTRUN_CWD = $$DESTDIR

debug_and_release:debug_and_release_target {
    # But in debug-and-release-target mode we don't want to cd into the debug/release
    # directory (e.g. if the test goes to foo/release/tst_thing.exe, we want to do
    # 'cd foo && release/tst_thing.exe', not 'cd foo/release && tst_thing.exe').

    XUNITTESTRUN_CWD ~= s/(release|debug)$//
    XUNITTEST_TARGET_DIR = $$relative_path($$absolute_path($$DESTDIR, $$OUT_PWD), $$absolute_path($$XUNITTESTRUN_CWD, $$OUT_PWD))
}

!isEmpty(XUNITTESTRUN_CWD):!contains(XUNITTESTRUN_CWD,^\\./?): \
    xunittest.commands = cd $$system_path($$XUNITTESTRUN_CWD) &&

unix {
    isEmpty(XUNITTEST_TARGET_DIR): XUNITTEST_TARGET_DIR = .

    mac:app_bundle: \
        xunittest.commands += $${XUNITTEST_TARGET_DIR}/$(QMAKE_TARGET).app/Contents/MacOS/$(QMAKE_TARGET)
    else: \
        xunittest.commands += $${XUNITTEST_TARGET_DIR}/$(QMAKE_TARGET)
} else {
    # Windows
    !isEmpty(XUNITTEST_TARGET_DIR): XUNITTEST_TARGET_DIR = $${XUNITTEST_TARGET_DIR}$${QMAKE_DIR_SEP}
    xunittest.commands += $${XUNITTEST_TARGET_DIR}$(TARGET)
}

# Allow for custom arguments to tests
xunittest.commands += -o $$system_path($$DESTDIR)$${QMAKE_DIR_SEP}$(QMAKE_TARGET)-xunit.xml,xunitxml

qtAddTargetEnv(xunittest.commands, QT)

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
