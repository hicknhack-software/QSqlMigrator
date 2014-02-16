TEMPLATE = subdirs
CONFIG += debug_and_release

win32-g++ {
CONFIG += c++11
QMAKE_CXXFLAGS_CXX11 -= -std=c++0x
QMAKE_CXXFLAGS_CXX11 += -std=c++11
}

SUBDIRS += \
	src \
	plugins \
	tests

plugins.depends = src
tests.depends = src plugins

OTHER_FILES += \
	README \
	CHANGES \
	LICENSE.GPL3 \
	LICENSE.LGPL
