CONFIG += debug_and_release
TEMPLATE = subdirs

!contains(QMAKE_EXTRA_TARGETS, xunittest) {
    prepareRecursiveTarget(xunittest)
    QMAKE_EXTRA_TARGETS += xunittest
}
