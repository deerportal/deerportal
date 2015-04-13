TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
SOURCES += main.cpp \
    game.cpp \
    tilemap.cpp

LIBS += -lsfml-window  -lsfml-system -lsfml-graphics

HEADERS += \
    game.h \
    tilemap.h

OTHER_FILES += \
    assets/img/zw-tilesets/_MAP.png
