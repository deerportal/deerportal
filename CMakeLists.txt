#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

# Here must be also current version - opposite to debian files which has latest release version

set(DEERPORTAL_MAJOR_VERSION 0)
set(DEERPORTAL_MINOR_VERSION 7)
set(DEERPORTAL_PATCH_VERSION 2)
set(DEERPORTAL_VERSION
  ${DEERPORTAL_MAJOR_VERSION}.${DEERPORTAL_MINOR_VERSION}.${DEERPORTAL_PATCH_VERSION})


# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project(myproject)

#target_compile_definitions(DeerPortal PRIVATE FOO=1 BAR=1)



add_definitions(-DDEERPORTAL_VERSION="${DEERPORTAL_VERSION}")
# Set version information in a config.h file
# configure_file(
#   "${PROJECT_SOURCE_DIR}/config.h.in"
#   "${PROJECT_BINARY_DIR}/config.h"
#   )
include_directories("${PROJECT_BINARY_DIR}")
include_directories(${YOUR_DIRECTORY})

# Define sources and executable
set(EXECUTABLE_NAME "DeerPortal")
add_executable(${EXECUTABLE_NAME}
    main.cpp
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
    particlesystem.cpp
    pile.cpp
    playerhud.cpp
    rotateelem.cpp
    rounddice.cpp
    selector.cpp
    soundfx.cpp
    textureholder.cpp
    tilemap.cpp)


# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


# CPack packaging
include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${DEERPORTAL_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${DEERPORTAL_MINOR_VERSION}.${DEERPORTAL_PATCH_VERSION}")
include(CPack)
