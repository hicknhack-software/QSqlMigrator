
SUBDIRS += \
        src \
        plugins \
        tests

include(_subdir.pri)

plugins.depends = src
tests.depends = src plugins

OTHER_FILES += \
        README.md \
        CHANGES \
        LICENSE.GPL3 \
        LICENSE.LGPL \
        .travis.yml \
        common.pri
