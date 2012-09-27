SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/dbg.cpp \
    src/ksilitunittest.cpp \
    src/jot.cpp \
    src/jottermodel.cpp

HEADERS += \
    src/mainwindow.h \
    src/dbg.h \
    src/defines.h \
    src/ksilitunittest.h \
    src/jot.h \
    src/jottermodel.h

FORMS += \
    src/mainwindow.ui


UI_DIR = build
MOC_DIR = build
OBJECTS_DIR = build
DESTDIR = bin

CONFIG += qtestlib
QT += xml

ksilit_install.path=/usr/local/bin/
ksilit_install.files=bin/ksilit
INSTALLS += ksilit_install
