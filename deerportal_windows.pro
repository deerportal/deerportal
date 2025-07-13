TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
SOURCES += src/main.cpp \
    src/game.cpp \
    src/tilemap.cpp \
    src/selector.cpp \
    src/playerhud.cpp \
    src/boardelem.cpp \
    src/boardelems.cpp \
    src/textureholder.cpp \
    src/hover.cpp \
    src/guiwindow.cpp \
    src/rounddice.cpp \
    src/guirounddice.cpp \
    src/grouphud.cpp \
    src/animation.cpp \
    src/animatedsprite.cpp \
    src/character.cpp \
    src/data.cpp \
    src/rotateelem.cpp \
    src/boarddiamond.cpp \
    src/boarddiamondseq.cpp \
    src/elem.cpp \
    src/soundfx.cpp \
    src/card.cpp \
    src/pile.cpp \
    src/cardslist.cpp \
    src/command.cpp \
    src/cardsdeck.cpp \
    src/particlesystem.cpp \
    src/calendar.cpp \
    src/bubble.cpp \
    src/banner.cpp \
    src/credits.cpp


#LIBS += -lsfml-window  -lsfml-system -lsfml-graphics -lsfml-audio
DESTDIR = builds/build_release_deerportal_win32

LIBS +=  -lsfml-system-s -lsfml-window-s   -lsfml-graphics-s -lsfml-audio-s -ljpeg -ljpeg -lglew32 -lfreetype -lgdi32 -lopengl32  -lwinmm -lbz2 -lpng -lOpenAL32 -lFLAC -lharfbuzz -lglib-2.0  -lintl -liconv -lgio-2.0 -lmingw32  -lm -luser32 -lgdi32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32  -lwinmm -ldxguid -ltiff -llzma -ljpeg -lz -lGLEW -lopengl32  -lgmp -lnettle -lws2_32 -lsndfile -lwinpthread -lpthread -lole32  -lvorbisfile -lvorbisenc -lvorbis -logg -lsfml-system-s   -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid 
DEFINES += "SFML_STATIC" "GLEW_STATIC=1" "AL_LIBTYPE_STATIC=1"
DEFINES += "DEERPORTAL_VERSION=$$(DEERPORTAL_VERSION)"
INCLUDEPATH += .
LIBPATH     += ~/opt/mxe/usr/i686-w64-mingw32.static/lib/ ~/opt/mxe/usr/lib/gcc/i686-w64-mingw32.static/4.9.3/

# Asset installation configuration
assets.path    = $${DESTDIR}/assets/
assets.files   = assets/*
INSTALLS        += assets

# Add PRE_TARGETDEPS for build-time copying (CI ENHANCEMENT)
win32 {
  assets_copy.commands = $(COPY_DIR) $$shell_path($$PWD/assets) $$shell_path($$OUT_PWD/assets)
  assets_copy.depends = FORCE
  QMAKE_EXTRA_TARGETS += assets_copy
  PRE_TARGETDEPS += assets_copy
}

# LICENSE file inclusion
license.path = $${DESTDIR}/
license.files = LICENSE
INSTALLS += license

# Add LICENSE to distribution files
OTHER_FILES += LICENSE
DISTFILES += LICENSE

HEADERS += \
    src/game.h \
    src/tilemap.h \
    src/selector.h \
    src/playerhud.h \
    src/boardelem.h \
    src/boardelems.h \
    src/textureholder.h \
    src/hover.h \
    src/guiwindow.h \
    src/elemsdescription.h \
    src/rounddice.h \
    src/guirounddice.h \
    src/grouphud.h \
    src/data.h \
    src/animation.h \
    src/animatedsprite.h \
    src/character.h \
    src/rotateelem.h \
    src/boarddiamond.h \
    src/boarddiamondseq.h \
    src/elem.h \
    src/soundfx.h \
    src/card.h \
    src/pile.h \
    src/cardslist.h \
    src/command.h \
    src/cardsdeck.h \
    src/calendar.h \
    src/particle.h \
    src/bubble.h \
    src/banner.h \
    src/credits.h



OTHER_FILES += \
    CREDITS.md \
    LICENSE
QMAKE_CXXFLAGS += -std=gnu++0x -Wpedantic

DISTFILES += \
    assets/shaders/blur.frag \
    assets/shaders/pixelate.frag \
    assets/shaders/dark.frag \
    LICENSE

QMAKE_CXXFLAGS += -std=gnu++0x -Wpedantic -mwindows
QMAKE_LFLAGS = -static -static-libgcc -static-libstdc++ -mwindows
TARGET = "Deer Portal"
win32:RC_ICONS += scripts/windows.ico
win32:RC_FILE += scripts/DeerPortal.rc
