
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# TARGET is mathpack, and TEMPLATE is lib, so the output of this build will
# be libmathpack.a
#
TARGET = mathpack
TEMPLATE = lib

INCLUDEPATH += ../include
DEPENDPATH += ../lib

win32:CONFIG(debug, debug|release): LIBS += -L../../lib -lqpgui
else:unix: LIBS += -L../../lib -lqpgui

CONFIG += staticlib

DEFINES += MATHPACK_LIBRARY

SOURCES += \
    randomop.cpp \
    mersenne.cpp \
    mpscore.cpp \
    testparm.cpp \
    resultfilemanager.cpp \
    leastcommult.cpp \
    factors.cpp \
    randmanager.cpp \
    testparmmanager.cpp

HEADERS += \
    mathpack.h\
    mathpack_global.h \
    randomop.h \
    randomc.h \
    mpscore.h \
    testparm.h \
    resultfile.h \
    resultfilemanager.h \
    leastcommult.h \
    factors.h \
    randmanager.h \
    testparmmanager.h

# Output the libqpgui.a file to the local lib directory
#
debug {
    DESTDIR = ../lib
}

release {
    DESTDIR = ../lib
}
