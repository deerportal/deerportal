TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
SOURCES += main.cpp \
    game.cpp \
    tilemap.cpp \
    selector.cpp \
    playerhud.cpp

LIBS += -lsfml-window  -lsfml-system -lsfml-graphics
DESTDIR = ../build_release_enfucraft

assets.path    = $${DESTDIR}/assets
assets.files   = assets/*

INSTALLS        += assets

HEADERS += \
    game.h \
    tilemap.h \
    selector.h \
    playerhud.h

OTHER_FILES += \
    assets/img/zw-tilesets/_MAP.png

RESOURCES += \
    images.qrc
