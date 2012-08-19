SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/dbg.cpp \
    src/ksilitunittest.cpp

HEADERS += \
    src/mainwindow.h \
    src/dbg.h \
    src/defines.h \
    src/ksilitunittest.h

FORMS += \
    src/mainwindow.ui


UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
DESTDIR = bin

CONFIG += qtestlib
