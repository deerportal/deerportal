TEMPLATE = app
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
    rounddice.cpp \
    guirounddice.cpp \
    grouphud.cpp \
    animation.cpp \
    animatedsprite.cpp \
    character.cpp \
    data.cpp \
    rotateelem.cpp \
    boarddiamond.cpp \
    boarddiamondseq.cpp \
    elem.cpp \
    soundfx.cpp \
    card.cpp \
    pile.cpp \
    cardslist.cpp \
    command.cpp \
    cardsdeck.cpp \
    particlesystem.cpp \
    calendar.cpp \
    bubble.cpp \
    banner.cpp \
    credits.cpp



#LIBS += -lsfml-window  -lsfml-system -lsfml-graphics -lsfml-audio
DESTDIR = builds/build_release_deerportal_win32

LIBS +=  -lsfml-system-s -lsfml-window-s   -lsfml-graphics-s -lsfml-audio-s -ljpeg -ljpeg -lglew32 -lfreetype -lgdi32 -lopengl32  -lwinmm -lbz2 -lpng -lOpenAL32 -lFLAC -lharfbuzz -lglib-2.0  -lintl -liconv -lgio-2.0 -lmingw32  -lm -luser32 -lgdi32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32  -lwinmm -ldxguid -ltiff -llzma -ljpeg -lz -lGLEW -lopengl32  -lgmp -lnettle -lws2_32 -lsndfile -lwinpthread -lpthread -lole32  -lvorbisfile -lvorbisenc -lvorbis -logg -lsfml-system-s   -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid 
DEFINES += "SFML_STATIC" "GLEW_STATIC=1" "AL_LIBTYPE_STATIC=1"
INCLUDEPATH += .
LIBPATH     += ~/opt/mxe/usr/i686-w64-mingw32.static/lib/ ~/opt/mxe/usr/lib/gcc/i686-w64-mingw32.static/4.9.3/

assets.path    = $${DESTDIR}/assets/
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
    elemsdescription.h \
    rounddice.h \
    guirounddice.h \
    grouphud.h \
    data.h \
    animation.h \
    animatedsprite.h \
    character.h \
    rotateelem.h \
    boarddiamond.h \
    boarddiamondseq.h \
    elem.h \
    soundfx.h \
    card.h \
    pile.h \
    cardslist.h \
    command.h \
    cardsdeck.h \
    calendar.h \
    particle.h \
    bubble.h \
    banner.h \
    credits.h



OTHER_FILES += \
    CREDITS.md
QMAKE_CXXFLAGS += -std=gnu++0x -Wpedantic

DISTFILES += \
    assets/shaders/blur.frag \
    assets/shaders/pixelate.frag \
    assets/shaders/dark.frag

QMAKE_CXXFLAGS += -std=gnu++0x -Wpedantic -mwindows
QMAKE_LFLAGS = -static -static-libgcc -static-libstdc++ -mwindows
TARGET = "Deer Portal"
win32:RC_ICONS += windows.ico
win32:RC_FILE += DeerPortal.rc
