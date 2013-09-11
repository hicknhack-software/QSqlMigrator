TEMPLATE = subdirs

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

