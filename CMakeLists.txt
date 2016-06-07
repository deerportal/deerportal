cmake_minimum_required(VERSION 2.8)

project(deerportal)

find_package(SFML 2.1 COMPONENTS graphics window audio system REQUIRED)

set(SOURCES
	animatedsprite.cpp
	animation.cpp
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

include_directories(${SFML_INCLUDE_DIR})
add_executable(deerportal ${SOURCES})
target_link_libraries(deerportal ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
