cmake_minimum_required(VERSION 3.0)

project(deerportal)

find_package(SFML 2.1 COMPONENTS graphics window audio system REQUIRED)

set(SOURCES
	animatedsprite.cpp
	animation.cpp
        banner.cpp
	boarddiamond.cpp
	boarddiamondseq.cpp
	boardelem.cpp
	boardelems.cpp
	bubble.cpp
	calendar.cpp
	card.cpp
	cardsdeck.cpp
	cardslist.cpp
	character.cpp
	command.cpp
	credits.cpp
	data.cpp
	elem.cpp
	game.cpp
	grouphud.cpp
	guirounddice.cpp
	guiwindow.cpp
	hover.cpp
	main.cpp
	particlesystem.cpp
	pile.cpp
	playerhud.cpp
	rotateelem.cpp
	rounddice.cpp
	selector.cpp
	soundfx.cpp
	textureholder.cpp
	tilemap.cpp
)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -static-libgcc -static-libgcc")
endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
include_directories(${SFML_INCLUDE_DIR})
add_executable(deerportal ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 11)
target_link_libraries(deerportal ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
