
include(../test.pri)

win32:QMAKE_POST_LINK = $$DESTDIR/$${TARGET}.exe
else:unix:QMAKE_POST_LINK = $$DESTDIR/$${TARGET}
