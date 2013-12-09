#-------------------------------------------------
#
# Project created by QtCreator 2011-12-16T14:03:53
#
#-------------------------------------------------

QT       -= gui

TARGET = mathpack
TEMPLATE = lib

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

HEADERS += mathpack.h\
        mathpack_global.h \
    randomop.h \
    randomc.h \
    mpscore.h \
    testparm.h \
    resultfile.h \
    resultfilemanager.h \
    leastcommult.h \
    factors.h

debug {
    DESTDIR = $$PWD/debug
}

release {
    DESTDIR = $$PWD/release
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


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../qpgui/release/ -lqpgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../qpgui/debug/ -lqpgui
else:symbian: LIBS += -lqpgui
else:unix: LIBS += -L$$PWD/../qpgui/ -lqpgui

INCLUDEPATH += $$PWD/../qpgui
DEPENDPATH += $$PWD/../qpgui

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../qpgui/release/qpgui.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../qpgui/debug/qpgui.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../qpgui/libqpgui.a











