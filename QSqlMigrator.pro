
SUBDIRS += src
!contains(CONFIG, NO_QSM_PLUGINS): {
    SUBDIRS += plugins
    plugins.depends = src

    !contains(CONFIG, NO_QSM_TESTS) {
        SUBDIRS += example
        example.depends = src plugins

        SUBDIRS += tests
        tests.depends = src plugins
    }
}

include(_subdir.pri)

OTHER_FILES += \
    README.md \
    CHANGES \
    LICENSE.GPL3 \
    LICENSE.LGPL \
    .travis.yml \
    appveyor.yml \
    common.pri
