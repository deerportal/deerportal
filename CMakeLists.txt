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

project(DeerPortal)

#target_compile_definitions(DeerPortal PRIVATE FOO=1 BAR=1)
add_definitions(-DDEERPORTAL_VERSION="${DEERPORTAL_VERSION}.${CMAKE_BUILD_TYPE}")
# Set version information in a config.h file
# configure_file(
#   "${PROJECT_SOURCE_DIR}/config.h.in"
#   "${PROJECT_BINARY_DIR}/config.h"
#   )
include_directories("${PROJECT_BINARY_DIR}")
# include_directories(${YOUR_DIRECTORY})  # Commented out - undefined variable

# Define sources and executable
set(EXECUTABLE_NAME "DeerPortal")

# Collect all source files
file(GLOB GAME_SOURCES 
    src/main.cpp
    src/game.cpp
    src/game-assets.cpp
    src/game-input.cpp
    src/game-renderer.cpp
    src/game-core.cpp
    src/game-state-manager.cpp
    src/game-animation-system.cpp
    src/window-manager.cpp
    src/cardnotification.cpp
    src/pile.cpp
    src/utils.cpp
    src/safe-asset-loader.cpp
    src/error-handler.cpp
)

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
    src/bubble.cpp
    src/bubble.h
    src/command.cpp
    src/command.h
    src/banner.cpp
    src/banner.h
    src/cardnotification.cpp
    src/cardnotification.h
    src/credits.cpp
    src/credits.h
    src/introshader.cpp
    src/introshader.h
    # Missing files that were causing linking errors
    src/elem.cpp
    src/boardelems.cpp
    src/boarddiamond.cpp
    src/animation.cpp
    src/cardslist.cpp
    src/data.cpp
    src/boardelem.cpp
    src/card.cpp
)

# Add Windows icon resource file
if(WIN32)
    list(APPEND OTHER_SOURCES scripts/DeerPortal.rc)
endif()

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
  MACOSX_FRAMEWORK_IDENTIFIER org.deerportal.DeerPortal
  MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/packaging/Info.plist"
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
  
  # Copy icon to Resources directory
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/packaging/DeerPortal.icns
    $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Resources/DeerPortal.icns
    COMMENT "Copying icon to macOS app bundle"
  )
  
  # Bundle SFML libraries for distribution
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks
    COMMENT "Creating Frameworks directory"
  )
  
  # Detect SFML library location and bundle appropriately
  if(DEFINED ENV{GITHUB_ACTIONS})
    # CI environment: Check for static libraries first (source build)
    if(EXISTS "/usr/local/lib/libsfml-system.a")
      message(STATUS "CI: Using static SFML libraries from source build - no bundling needed")
    elseif(EXISTS "/opt/homebrew/lib/libsfml-system.dylib")
      # ARM macOS with Homebrew SFML
      add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        /opt/homebrew/lib/libsfml-graphics.3.0.dylib
        /opt/homebrew/lib/libsfml-audio.3.0.dylib
        /opt/homebrew/lib/libsfml-network.3.0.dylib
        /opt/homebrew/lib/libsfml-window.3.0.dylib
        /opt/homebrew/lib/libsfml-system.3.0.dylib
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/
        COMMENT "Bundling Homebrew SFML libraries (ARM CI)"
      )
    else()
      message(STATUS "CI: No dynamic SFML libraries found, assuming static build")
    endif()
  else()
    # Local development: Try Homebrew paths first, then fallback
    if(EXISTS "/opt/homebrew/opt/sfml/lib/libsfml-system.dylib")
      add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        /opt/homebrew/opt/sfml/lib/libsfml-graphics.3.0.dylib
        /opt/homebrew/opt/sfml/lib/libsfml-audio.3.0.dylib
        /opt/homebrew/opt/sfml/lib/libsfml-network.3.0.dylib
        /opt/homebrew/opt/sfml/lib/libsfml-window.3.0.dylib
        /opt/homebrew/opt/sfml/lib/libsfml-system.3.0.dylib
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/
        COMMENT "Bundling SFML libraries (Homebrew)"
      )
    elseif(EXISTS "/usr/local/lib/libsfml-system.dylib")
      add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        /usr/local/lib/libsfml-graphics.3.0.dylib
        /usr/local/lib/libsfml-audio.3.0.dylib
        /usr/local/lib/libsfml-network.3.0.dylib
        /usr/local/lib/libsfml-window.3.0.dylib
        /usr/local/lib/libsfml-system.3.0.dylib
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/
        COMMENT "Bundling SFML libraries (/usr/local)"
      )
    else()
      message(WARNING "SFML dynamic libraries not found for bundling")
    endif()
  endif()
  
  # Bundle additional dependencies that SFML needs (only for dynamic builds)
  if(DEFINED ENV{GITHUB_ACTIONS})
    # CI environment: Only bundle dependencies for ARM Homebrew builds
    if(EXISTS "/opt/homebrew/lib/libsfml-system.dylib" AND EXISTS "/opt/homebrew/opt/freetype/lib/libfreetype.6.dylib")
      add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        /opt/homebrew/opt/freetype/lib/libfreetype.6.dylib
        /opt/homebrew/opt/libpng/lib/libpng16.16.dylib
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/
        COMMENT "Bundling SFML dependencies (ARM CI)"
      )
    endif()
  else()
    # Local development: Try to bundle dependencies if found
    if(EXISTS "/opt/homebrew/opt/freetype/lib/libfreetype.6.dylib")
      add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        /opt/homebrew/opt/freetype/lib/libfreetype.6.dylib
        /opt/homebrew/opt/libpng/lib/libpng16.16.dylib
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/
        COMMENT "Bundling SFML dependencies"
      )
    endif()
  endif()
  
  # Fix library paths for distribution (only needed for dynamic builds)
  if(NOT (DEFINED ENV{GITHUB_ACTIONS} AND EXISTS "/usr/local/lib/libsfml-system.a"))
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
      COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-graphics.3.0.dylib @executable_path/../Frameworks/libsfml-graphics.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-audio.3.0.dylib @executable_path/../Frameworks/libsfml-audio.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-network.3.0.dylib @executable_path/../Frameworks/libsfml-network.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-window.3.0.dylib @executable_path/../Frameworks/libsfml-window.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMAND install_name_tool -change /usr/local/lib/libsfml-graphics.3.0.dylib @executable_path/../Frameworks/libsfml-graphics.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMAND install_name_tool -change /usr/local/lib/libsfml-audio.3.0.dylib @executable_path/../Frameworks/libsfml-audio.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMAND install_name_tool -change /usr/local/lib/libsfml-network.3.0.dylib @executable_path/../Frameworks/libsfml-network.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMAND install_name_tool -change /usr/local/lib/libsfml-window.3.0.dylib @executable_path/../Frameworks/libsfml-window.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMAND install_name_tool -change /usr/local/lib/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
      COMMENT "Fixing library paths for distribution"
    )
  endif()
  
  # Fix library identities and internal dependencies (only for dynamic builds)
  if(NOT (DEFINED ENV{GITHUB_ACTIONS} AND EXISTS "/usr/local/lib/libsfml-system.a"))
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
      COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-graphics.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-graphics.3.0.dylib 2>/dev/null || true
      COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-audio.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-audio.3.0.dylib 2>/dev/null || true
      COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-network.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-network.3.0.dylib 2>/dev/null || true
      COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-window.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-window.3.0.dylib 2>/dev/null || true
      COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-system.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-system.3.0.dylib 2>/dev/null || true
      COMMENT "Fixing library identities"
    )
  endif()
  
  # Fix dependency library identities and dependencies (only for dynamic builds)
  if(NOT (DEFINED ENV{GITHUB_ACTIONS} AND EXISTS "/usr/local/lib/libsfml-system.a"))
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
      COMMAND install_name_tool -id @executable_path/../Frameworks/libfreetype.6.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libfreetype.6.dylib 2>/dev/null || true
      COMMAND install_name_tool -id @executable_path/../Frameworks/libpng16.16.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libpng16.16.dylib 2>/dev/null || true
      COMMENT "Fixing dependency library identities"
    )
    
    # Fix internal library dependencies
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
      COMMAND install_name_tool -change @rpath/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-graphics.3.0.dylib 2>/dev/null || true
      COMMAND install_name_tool -change @rpath/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-audio.3.0.dylib 2>/dev/null || true
      COMMAND install_name_tool -change @rpath/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-network.3.0.dylib 2>/dev/null || true
      COMMAND install_name_tool -change @rpath/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-window.3.0.dylib 2>/dev/null || true
      COMMENT "Fixing internal library dependencies"
    )
    
    # Fix @rpath dependencies that could load system libraries
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
      COMMAND install_name_tool -change @rpath/libsfml-window.3.0.dylib @executable_path/../Frameworks/libsfml-window.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-graphics.3.0.dylib 2>/dev/null || true
      COMMAND install_name_tool -change @rpath/libsfml-graphics.3.0.dylib @executable_path/../Frameworks/libsfml-graphics.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-audio.3.0.dylib 2>/dev/null || true
      COMMENT "Fixing @rpath dependencies"
    )
    
    # Fix SFML dependencies on freetype and libpng
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
      COMMAND install_name_tool -change /opt/homebrew/opt/freetype/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-graphics.3.0.dylib 2>/dev/null || true
      COMMAND install_name_tool -change /opt/homebrew/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libfreetype.6.dylib 2>/dev/null || true
      COMMENT "Fixing SFML dependencies on bundled libraries"
    )
  endif()
  
  # Sign the app bundle with ad-hoc signature for macOS compatibility
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND codesign --force --deep --sign - $<TARGET_BUNDLE_DIR:${EXECUTABLE_NAME}>
    COMMENT "Code signing app bundle"
  )
  
  # For macOS, let runtime detection handle the path
  add_definitions(-DASSETS_PATH="assets/")
  add_definitions(-DBASE_PATH="app_bundle")
elseif(WIN32)
  # Windows: Copy assets to build directory for immediate use
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/assets
    COMMENT "Copying assets to Windows build directory"
  )
  
  # Windows-specific asset path definitions
  add_definitions(-DASSETS_PATH="assets/")
  add_definitions(-DBASE_PATH=".")
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

  # Asset path definitions for Linux/Unix
  set (ASSETS_BASE_PATH share/games/deerportal)
  add_definitions(-DASSETS_PATH="assets/")
endif()

# Install target
IF (WIN32)
  set (ASSETS_BASE_PATH data)
  install(TARGETS ${EXECUTABLE_NAME} DESTINATION .)
  install(DIRECTORY assets DESTINATION .)
  install(FILES LICENSE DESTINATION .)
ELSE()
  if(APPLE)
    # For macOS, install the app bundle
    install(TARGETS ${EXECUTABLE_NAME} DESTINATION Applications)
  else()
    # For Linux/Unix
    set (ASSETS_BASE_PATH share/games/deerportal)
    install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
    install(DIRECTORY assets DESTINATION ${ASSETS_BASE_PATH})
  endif()
ENDIF()

# CPack packaging setup (needed for all platforms)
include(InstallRequiredSystemLibraries)

# Basic package information
set(CPACK_PACKAGE_NAME "DeerPortal")
set(CPACK_PACKAGE_VENDOR "DeerPortal Team")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Game About How Human Can Be Upgraded to the Deer")
set(CPACK_PACKAGE_DESCRIPTION "DeerPortal - An immersive game experience where players embark on a journey of transformation, exploring what it means to upgrade from human to deer.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/deerportal/deerportal")
set(CPACK_PACKAGE_CONTACT "https://github.com/deerportal/deerportal")

# Version information from version.txt
set(CPACK_PACKAGE_VERSION_MAJOR "${DEERPORTAL_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${DEERPORTAL_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${DEERPORTAL_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "${DEERPORTAL_VERSION}")

# Platform-specific packaging
if(APPLE)
    # macOS: Create proper app bundle and DMG using pure CMake
    set(CPACK_GENERATOR "")  # Disable CPack generators for macOS
    
    # File name format: DeerPortal-0.9.0-macOS.dmg  
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-macOS")
    
    # DMG creation variables
    set(DMG_NAME "${CPACK_PACKAGE_FILE_NAME}")
    set(DMG_PATH "${CMAKE_BINARY_DIR}/${DMG_NAME}.dmg")
    set(DMG_TEMP_DIR "${CMAKE_BINARY_DIR}/dmg_temp")
    
    # Custom target to create signed DMG
    add_custom_target(create_dmg
        COMMENT "Creating macOS DMG package: ${DMG_NAME}.dmg"
        DEPENDS ${EXECUTABLE_NAME}
        
        # Clean previous temp directory and DMG files
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${DMG_TEMP_DIR}"
        COMMAND ${CMAKE_COMMAND} -E remove -f "${DMG_PATH}"
        COMMAND ${CMAKE_COMMAND} -E remove -f "${CMAKE_BINARY_DIR}/${DMG_NAME}-temp.dmg"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DMG_TEMP_DIR}"
        
        # Copy app bundle to temp directory
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
                "$<TARGET_BUNDLE_DIR:${EXECUTABLE_NAME}>" 
                "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app"
        
        # Sign the app bundle (suppress warnings)
        COMMAND codesign --force --deep --sign - "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app" 2>/dev/null || true
        
        # Fix library identities in DMG version (suppress warnings)
        COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-graphics.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-graphics.3.0.dylib" 2>/dev/null || true
        COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-audio.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-audio.3.0.dylib" 2>/dev/null || true
        COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-network.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-network.3.0.dylib" 2>/dev/null || true
        COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-window.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-window.3.0.dylib" 2>/dev/null || true
        COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-system.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-system.3.0.dylib" 2>/dev/null || true
        
        # Fix dependency library identities in DMG version (suppress warnings)
        COMMAND install_name_tool -id @executable_path/../Frameworks/libfreetype.6.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libfreetype.6.dylib" 2>/dev/null || true
        COMMAND install_name_tool -id @executable_path/../Frameworks/libpng16.16.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libpng16.16.dylib" 2>/dev/null || true
        
        # Fix internal library dependencies in DMG version (suppress warnings)
        COMMAND install_name_tool -change @rpath/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-graphics.3.0.dylib" 2>/dev/null || true
        COMMAND install_name_tool -change @rpath/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-audio.3.0.dylib" 2>/dev/null || true
        COMMAND install_name_tool -change @rpath/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-network.3.0.dylib" 2>/dev/null || true
        COMMAND install_name_tool -change @rpath/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-window.3.0.dylib" 2>/dev/null || true
        
        # Fix @rpath dependencies in DMG version (suppress warnings)
        COMMAND install_name_tool -change @rpath/libsfml-window.3.0.dylib @executable_path/../Frameworks/libsfml-window.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-graphics.3.0.dylib" 2>/dev/null || true
        COMMAND install_name_tool -change @rpath/libsfml-graphics.3.0.dylib @executable_path/../Frameworks/libsfml-graphics.3.0.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-audio.3.0.dylib" 2>/dev/null || true
        
        # Fix SFML dependencies on freetype and libpng in DMG version (suppress warnings)
        COMMAND install_name_tool -change /opt/homebrew/opt/freetype/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libsfml-graphics.3.0.dylib" 2>/dev/null || true
        COMMAND install_name_tool -change /opt/homebrew/opt/libpng/lib/libpng16.16.dylib @executable_path/../Frameworks/libpng16.16.dylib "${DMG_TEMP_DIR}/${EXECUTABLE_NAME}.app/Contents/Frameworks/libfreetype.6.dylib" 2>/dev/null || true
        
        # Copy background image
        COMMAND ${CMAKE_COMMAND} -E copy 
                "${CMAKE_SOURCE_DIR}/packaging/dmg_background.png"
                "${DMG_TEMP_DIR}/.background.png"
        
        # Create Applications symlink
        COMMAND ${CMAKE_COMMAND} -E create_symlink 
                "/Applications" 
                "${DMG_TEMP_DIR}/Applications"
        
        # Create temporary DMG
        COMMAND hdiutil create -volname "${DMG_NAME}" 
                -srcfolder "${DMG_TEMP_DIR}" 
                -ov -format UDRW 
                "${CMAKE_BINARY_DIR}/${DMG_NAME}-temp.dmg"
        
        # Mount DMG for customization
        COMMAND ${CMAKE_COMMAND} -E echo "Mounting DMG for customization..."
        
        # Convert to final compressed DMG  
        COMMAND hdiutil convert "${CMAKE_BINARY_DIR}/${DMG_NAME}-temp.dmg" 
                -format UDBZ -o "${DMG_PATH}"
        
        # Clean up
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${DMG_TEMP_DIR}"
        COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/${DMG_NAME}-temp.dmg"
        
        # Success message
        COMMAND ${CMAKE_COMMAND} -E echo "✅ DMG created successfully: ${DMG_NAME}.dmg"
        COMMAND ls -lh "${DMG_PATH}"
        
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    # Add dmg target for macOS DMG creation
    add_custom_target(dmg
        DEPENDS create_dmg
        COMMENT "Creating macOS DMG package"
    )
    
elseif(WIN32)
    # Windows installer configuration
    set(CPACK_GENERATOR "NSIS;ZIP")
    
    # Explicitly enable ZIP binary generation (fix for missing ZIP files)
    set(CPACK_BINARY_ZIP ON)
    set(CPACK_BINARY_NSIS ON)
    
    set(CPACK_NSIS_DISPLAY_NAME "DeerPortal")
    set(CPACK_NSIS_PACKAGE_NAME "DeerPortal")
    set(CPACK_NSIS_CONTACT "https://github.com/deerportal/deerportal")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/deerportal/deerportal")
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Windows")
    
    # License file for installer
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    
    # Start Menu shortcuts and desktop integration
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_CREATE_ICONS_EXTRA 
        "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\DeerPortal.lnk' '$INSTDIR\\\\DeerPortal.exe'"
    )
    set(CPACK_NSIS_DELETE_ICONS_EXTRA 
        "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\DeerPortal.lnk'"
    )
    
else()
    # Linux packaging
    set(CPACK_GENERATOR "TGZ;DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "DeerPortal Team")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    # Remove SFML dependency since we build statically
    # set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-dev (>= 3.0)")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.17), libgcc-s1 (>= 3.0), libstdc++6 (>= 5.2)")
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Linux")
endif()

# Source package configuration
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Source")
set(CPACK_SOURCE_IGNORE_FILES
    "/build/"
    "/\\\\.git/"
    "/\\\\.github/"
    "/\\\\.data/"
    "\\\\.gitignore"
    "\\\\.DS_Store"
    "/ai-docs/"
    ".*\\\\.dmg$"
    ".*\\\\.exe$"
    ".*\\\\.deb$"
    ".*\\\\.tar\\\\.gz$"
)

# Include CPack for all platforms
include(CPack)
