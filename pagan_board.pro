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
    guirounddice.cpp \
    grouphud.cpp \
    animation.cpp \
    animatedsprite.cpp \
    character.cpp \
    data.cpp

LIBS += -lsfml-window  -lsfml-system -lsfml-graphics -lsfml-audio
DESTDIR = ../build_release_pagan_board

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
    guirounddice.h \
    grouphud.h \
    data.h \
    animation.h \
    animatedsprite.h \
    character.h

RESOURCES += \
    images.qrc

OTHER_FILES += \
    CREDITS.md
QMAKE_CXXFLAGS += -std=gnu++0x
