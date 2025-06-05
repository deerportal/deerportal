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
    src/cardnotification.cpp
    src/cardnotification.h
    src/credits.cpp
    src/credits.h
    src/introshader.cpp
    src/introshader.h
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
  
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    /opt/homebrew/opt/sfml/lib/libsfml-graphics.3.0.dylib
    /opt/homebrew/opt/sfml/lib/libsfml-audio.3.0.dylib
    /opt/homebrew/opt/sfml/lib/libsfml-network.3.0.dylib
    /opt/homebrew/opt/sfml/lib/libsfml-window.3.0.dylib
    /opt/homebrew/opt/sfml/lib/libsfml-system.3.0.dylib
    $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/
    COMMENT "Bundling SFML libraries"
  )
  
  # Bundle additional dependencies that SFML needs
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    /opt/homebrew/opt/freetype/lib/libfreetype.6.dylib
    /opt/homebrew/opt/libpng/lib/libpng16.16.dylib
    $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/
    COMMENT "Bundling SFML dependencies"
  )
  
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-graphics.3.0.dylib @executable_path/../Frameworks/libsfml-graphics.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
    COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-audio.3.0.dylib @executable_path/../Frameworks/libsfml-audio.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
    COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-network.3.0.dylib @executable_path/../Frameworks/libsfml-network.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
    COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-window.3.0.dylib @executable_path/../Frameworks/libsfml-window.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
    COMMAND install_name_tool -change /opt/homebrew/opt/sfml/lib/libsfml-system.3.0.dylib @executable_path/../Frameworks/libsfml-system.3.0.dylib $<TARGET_FILE:${EXECUTABLE_NAME}> 2>/dev/null || true
    COMMENT "Fixing library paths for distribution"
  )
  
  # Fix library identities and internal dependencies
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-graphics.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-graphics.3.0.dylib 2>/dev/null || true
    COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-audio.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-audio.3.0.dylib 2>/dev/null || true
    COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-network.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-network.3.0.dylib 2>/dev/null || true
    COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-window.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-window.3.0.dylib 2>/dev/null || true
    COMMAND install_name_tool -id @executable_path/../Frameworks/libsfml-system.3.0.dylib $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/../Frameworks/libsfml-system.3.0.dylib 2>/dev/null || true
    COMMENT "Fixing library identities"
  )
  
  # Fix dependency library identities and dependencies
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
  
  # Sign the app bundle with ad-hoc signature for macOS compatibility
  add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND codesign --force --deep --sign - $<TARGET_BUNDLE_DIR:${EXECUTABLE_NAME}>
    COMMENT "Code signing app bundle"
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
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "DeerPortal")
    set(CPACK_NSIS_PACKAGE_NAME "DeerPortal")
    set(CPACK_NSIS_CONTACT "https://github.com/deerportal/deerportal")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/deerportal/deerportal")
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Windows")
    
else()
    # Linux packaging
    set(CPACK_GENERATOR "TGZ;DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "DeerPortal Team")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-dev (>= 3.0)")
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
