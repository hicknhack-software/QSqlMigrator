CONFIG += debug_and_release
TEMPLATE = subdirs

!contains(QMAKE_EXTRA_TARGETS, xunittest) {
    for(subdir, SUBDIRS) {
        subdir_config = $$eval($${subdir}.CONFIG)
        xunittest.recurse += $$subdir
    }

    xunittest.CONFIG = recursive
    xunittest.recurse_target = xunittest

    QMAKE_EXTRA_TARGETS += xunittest
}
