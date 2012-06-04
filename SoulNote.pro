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

HEADERS += \
    soulnote.h \
    mainconstants.h \
    textedit.h \
    findwidget.h \
    about.h

SOURCES += \
    soulnote.cpp \
    main.cpp \
    textedit.cpp \
    findwidget.cpp \
    about.cpp

FORMS += \
    soulnote.ui \
    about.ui

RESOURCES += \
    resource.qrc
