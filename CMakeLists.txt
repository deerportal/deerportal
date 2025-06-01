#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)

# To unify version handling
file (STRINGS "version.txt" DEERPORTAL_VERSION)
string(REGEX MATCH "^([0-9]+)" DEERPORTAL_VERSION_MAJOR ${DEERPORTAL_VERSION})
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\2" DEERPORTAL_VERSION_MINOR ${DEERPORTAL_VERSION})
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+.*)" "\\3" DEERPORTAL_VERSION_PATCH ${DEERPORTAL_VERSION})
MESSAGE( STATUS "DEERPORTAL_VERSION:         " ${DEERPORTAL_VERSION} )
MESSAGE( STATUS "DEERPORTAL_VERSION_MAJOR:         " ${DEERPORTAL_VERSION_MAJOR} )
MESSAGE( STATUS "DEERPORTAL_VERSION_MINOR:         " ${DEERPORTAL_VERSION_MINOR} )
MESSAGE( STATUS "DEERPORTAL_VERSION_PATCH:         " ${DEERPORTAL_VERSION_PATCH} )

# Configure C++ flags for different build types
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "Flags used by the C++ compiler during debug builds.")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto" CACHE STRING "Flags used by the C++ compiler during release builds.")
# For Apple Clang, -flto might need to be -flto=thin depending on linker, adjust if necessary
if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
  # For older Clang, you might need -fuse-ld=lld if default linker doesn't support LTO well
endif()

# Set default build type to Release if not specified by the user
# To choose a build type, use: cmake -DCMAKE_BUILD_TYPE=Debug .. or cmake -DCMAKE_BUILD_TYPE=Release ..
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release). Default is Release." FORCE)
endif()

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
MESSAGE(STATUS "C++ Flags (Release): ${CMAKE_CXX_FLAGS_RELEASE}")
MESSAGE(STATUS "C++ Flags (Debug): ${CMAKE_CXX_FLAGS_DEBUG}")

# Option to control FPS counter visibility in all builds
option(SHOW_FPS_COUNTER "Enable FPS counter in all builds" OFF)
if(SHOW_FPS_COUNTER)
  add_definitions(-DDEERPORTAL_SHOW_FPS_COUNTER)
  MESSAGE(STATUS "FPS Counter will be visible in this build (DEERPORTAL_SHOW_FPS_COUNTER defined)")
else()
  MESSAGE(STATUS "FPS Counter will be hidden unless in Debug build (NDEBUG not defined)")
endif()

project(myproject)

#target_compile_definitions(DeerPortal PRIVATE FOO=1 BAR=1)
add_definitions(-DDEERPORTAL_VERSION="${DEERPORTAL_VERSION}.${CMAKE_BUILD_TYPE}")
# Set version information in a config.h file
# configure_file(
#   "${PROJECT_SOURCE_DIR}/config.h.in"
#   "${PROJECT_BINARY_DIR}/config.h"
#   )
include_directories("${PROJECT_BINARY_DIR}")
include_directories(${YOUR_DIRECTORY})

# Define sources and executable
set(EXECUTABLE_NAME "DeerPortal")

# Main game files
set(GAME_SOURCES
    src/main.cpp
    src/game.cpp
    src/game.h
    # New modular files
    src/game-assets.cpp
    src/game-assets.h
    src/game-input.cpp
    src/game-input.h
    src/game-renderer.cpp
    src/game-renderer.h
    src/game-core.cpp
    src/game-core.h
)

# Other source files
file(GLOB OTHER_SOURCES 
    src/tilemap.cpp
    src/tilemap.h
    src/selector.cpp
    src/selector.h
    src/playerhud.cpp
    src/playerhud.h
    src/textureholder.cpp
    src/textureholder.h
    src/hover.cpp
    src/hover.h
    src/guiwindow.cpp
    src/guiwindow.h
    src/rounddice.cpp
    src/rounddice.h
    src/guirounddice.cpp
    src/guirounddice.h
    src/grouphud.cpp
    src/grouphud.h
    src/animatedsprite.cpp
    src/animatedsprite.h
    src/character.cpp
    src/character.h
    src/rotateelem.cpp
    src/rotateelem.h
    src/boarddiamondseq.cpp
    src/boarddiamondseq.h
    src/soundfx.cpp
    src/soundfx.h
    src/cardsdeck.cpp
    src/cardsdeck.h
    src/calendar.cpp
    src/calendar.h
    src/particle.cpp
    src/particle.h
    src/bubble.cpp
    src/bubble.h
    src/command.cpp
    src/command.h
    src/banner.cpp
    src/banner.h
    src/credits.cpp
    src/credits.h
    # Missing files that were causing linking errors
    src/particlesystem.cpp
    src/elem.cpp
    src/boardelems.cpp
    src/boarddiamond.cpp
    src/animation.cpp
    src/cardslist.cpp
    src/data.cpp
    src/boardelem.cpp
    src/card.cpp
)

add_executable(${EXECUTABLE_NAME} ${GAME_SOURCES} ${OTHER_SOURCES})

# Find SFML 3.0 using modern CMake approach
# Try to find SFML 3.0, be flexible with shared/static configuration
find_package(SFML 3 COMPONENTS System Window Graphics Audio Network QUIET)

# If not found, try alternative approaches
if(NOT SFML_FOUND)
  # Try with explicit static preference for CI builds
  message(STATUS "SFML 3.0 not found with default config, trying alternative approaches...")
  set(SFML_STATIC_LIBRARIES ON)
  find_package(SFML 3 COMPONENTS System Window Graphics Audio Network QUIET)
endif()

# Final required check
if(NOT SFML_FOUND)
  find_package(SFML 3 COMPONENTS System Window Graphics Audio Network REQUIRED)
endif()

if(SFML_FOUND)
  message(STATUS "SFML 3.0 found successfully")
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio SFML::Network)
  target_include_directories(${EXECUTABLE_NAME} PRIVATE ${SFML_INCLUDE_DIRS})
endif()

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
  RESOURCE "${RESOURCE_FILES}"
)

# macOS App Bundle: Copy assets into the bundle
if(APPLE)
  # Copy assets into the app bundle Resources directory
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Resources/assets
    COMMENT "Copying assets to macOS app bundle"
  )
  
  # For macOS, let runtime detection handle the path
  add_definitions(-DASSETS_PATH="assets/")
  add_definitions(-DBASE_PATH="app_bundle")
else()
  # Original logic for other platforms
  # Ensure BASE_PATH is set correctly for non-Apple builds if using CMAKE_INSTALL_PREFIX
  if(DEFINED CMAKE_INSTALL_PREFIX)
    add_definitions(-DBASE_PATH="${CMAKE_INSTALL_PREFIX}")
  else()
    # Fallback or default if CMAKE_INSTALL_PREFIX is not always set
    # This might need adjustment based on how non-packaged versions are run
    add_definitions(-DBASE_PATH=".") 
  endif()

  IF (WIN32)
    set (ASSETS_BASE_PATH data)
    add_definitions(-DASSETS_PATH="assets/")
  ELSE()
    set (ASSETS_BASE_PATH share/games/deerportal)
    add_definitions(-DASSETS_PATH="assets/")
  ENDIF()
endif()

# Install target
IF (WIN32)
  set (ASSETS_BASE_PATH data)
  install(TARGETS ${EXECUTABLE_NAME} DESTINATION .)
  install(DIRECTORY assets DESTINATION "${ASSETS_BASE_PATH}/")
ELSE()
  if(APPLE)
    # For macOS, install the app bundle
    install(TARGETS ${EXECUTABLE_NAME} DESTINATION Applications)
  else()
    # For Linux/Unix
    set (ASSETS_BASE_PATH share/games/deerportal)
    install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
    install(DIRECTORY assets DESTINATION "${CMAKE_INSTALL_PREFIX}/${ASSETS_BASE_PATH}/")
  endif()
ENDIF()

# CPack packaging
include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${DEERPORTAL_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${DEERPORTAL_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${DEERPORTAL_VERSION_PATCH}")
include(CPack)
