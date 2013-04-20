SOURCES += \
    src/main.cpp \
    src/dbg.cpp \
    src/ksilitunittest.cpp \
    src/mainwindow.cpp \
    src/jot.cpp \
    src/jotter.cpp \
    src/database.cpp

HEADERS += \
    src/dbg.h \
    src/defines.h \
    src/ksilitunittest.h \
    src/mainwindow.h \
    src/jot.h \
    src/jotter.h \
    src/database.h

FORMS += \
    src/mainwindow.ui

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
DESTDIR = bin

CONFIG += qtestlib
QT += xml

ksilit_install.path = /usr/local/bin/
ksilit_install.files = bin/ksilit
INSTALLS += ksilit_install
