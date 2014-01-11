# There are no QT gui elements in this library, so let's delete them
# from the build.
#
QT       -= gui

# TARGET is mathpack, and TEMPLATE is lib, so the output of this build will
# be libmathpack.a
#
TARGET = mathpack
TEMPLATE = lib

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../lib

LIBS += -L$$PWD/../lib/libqpgui.a

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += staticlib

DEFINES += MATHPACK_LIBRARY

SOURCES += mathpack.cpp \
    randomop.cpp \
    mersenne.cpp \
    mpscore.cpp \
    testparm.cpp \
    resultfilemanager.cpp \
    leastcommult.cpp \
    factors.cpp

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
    factors.h

# Output the libqpgui.a file to the directory immediately above the
# respective build file.
#
debug {
    DESTDIR = ../lib
}

release {
    DESTDIR = ../lib
}

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE2F8CEF6
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = mathpack.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

