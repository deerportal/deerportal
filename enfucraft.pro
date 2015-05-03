TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
SOURCES += main.cpp \
    game.cpp \
    tilemap.cpp \
    selector.cpp \
    playerhud.cpp \
    boardelem.cpp \
    boardelems.cpp \
    textureholder.cpp \
    hover.cpp \
    guiwindow.cpp \
    purchaseguielem.cpp \
    guichoosebuilding.cpp \
    rounddice.cpp \
    guirounddice.cpp

LIBS += -lsfml-window  -lsfml-system -lsfml-graphics -lsfml-audio
DESTDIR = ../build_release_enfucraft

assets.path    = $${DESTDIR}/assets
assets.files   = assets/*

INSTALLS        += assets

HEADERS += \
    game.h \
    tilemap.h \
    selector.h \
    playerhud.h \
    boardelem.h \
    boardelems.h \
    textureholder.h \
    hover.h \
    guiwindow.h \
    purchaseguielem.h \
    guichoosebuilding.h \
    elemsdescription.h \
    rounddice.h \
    guirounddice.h

RESOURCES += \
    images.qrc

OTHER_FILES += \
    CREDITS.md
