TEMPLATE = app

QT += core gui

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
DESTDIR = bin

TARGET = SoulNote
target.path = $$PREFIX/bin
INSTALLS += target

QMAKE_CXXFLAGS += -march=core2
win32 {
    QMAKE_LFLAGS += -static-libgcc
}
RC_FILE = app.rc

HEADERS += \
    soulnote.h \
    mainconstants.h \
    textedit.h \
    findwidget.h \
    about.h \
    qtwin.h \
    preferences.h

SOURCES += \
    soulnote.cpp \
    main.cpp \
    textedit.cpp \
    findwidget.cpp \
    about.cpp \
    qtwin.cpp \
    preferences.cpp

FORMS += \
    soulnote.ui \
    about.ui \
    preferences.ui

RESOURCES += \
    resource.qrc
