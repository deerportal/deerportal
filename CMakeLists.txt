#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 3.5)
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

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

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
file( GLOB SRCS src/*.cpp src/*.h )
add_executable(${EXECUTABLE_NAME} ${SRCS})

# Find SFML 3.0 using modern CMake approach
find_package(SFML 3 COMPONENTS System Window Graphics Audio Network REQUIRED)
if(SFML_FOUND)
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio SFML::Network)
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
  
  # For macOS, let runtime detection handle the path (empty ASSETS_PATH)
  add_definitions(-DASSETS_PATH="")
  add_definitions(-DBASE_PATH="app_bundle")
else()
  # Original logic for other platforms
  add_definitions(-DBASE_PATH="${CMAKE_INSTALL_PREFIX}")
  IF (WIN32)
    set (ASSETS_BASE_PATH data)
    add_definitions(-DASSETS_PATH="${ASSETS_BASE_PATH}/")
  ELSE()
    set (ASSETS_BASE_PATH share/games/deerportal)
    add_definitions(-DASSETS_PATH="${CMAKE_INSTALL_PREFIX}/${ASSETS_BASE_PATH}/")
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
set(CPACK_PACKAGE_VERSION_MINOR "${DEERPORTAL_VERSION_MINOR}.${DEERPORTAL_VERSION_PATCH}")
include(CPack)
