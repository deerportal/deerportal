#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
#set(SFML_STATIC_LIBRARIES TRUE)

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

project(deerportal)

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

IF(WIN32)
	#Now we are looking for "sfml-main.lib".
	#Because we need it if we give ADD_EXECUTABLE the WIN32 switch to creat a GUI application (that one without a cmd promt)
	#set(SFML_DIR "C:/Users/blusz/Dev/SFML-2.5.1/")
	#set(SFML_DIR "C:/Users/blusz/Dev/SFML-2.6-x64/lib/cmake/SFML")
	set(SFML_DIR "C:/Users/blusz/Dev/SFML-2.6-x64/")
	FIND_LIBRARY( SFML_MAIN_LIBRARY
		NAMES sfml-main
		PATH_SUFFIXES lib64 lib
		PATHS ${SFML_LIBRARY_SEARCH_DIR}
	)	
	LIST(APPEND SFML_LIBRARY ${SFML_MAIN_LIBRARY})
ENDIF(WIN32)


find_package(SFML 2 COMPONENTS graphics audio REQUIRED)

IF (WIN32)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

add_executable(${EXECUTABLE_NAME} WIN32 ${SRCS}

	${CMAKE_CURRENT_SOURCE_DIR}/scripts/DeerPortal.rc
  )

ELSE()
add_executable(${EXECUTABLE_NAME}  ${SRCS}

	
  )
ENDIF()

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
# find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
  MACOSX_BUNDLE TRUE
  MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
  RESOURCE "${RESOURCE_FILES}"
)

# Install target



IF (WIN32)
  set (ASSETS_BASE_PATH data)
  install(TARGETS ${EXECUTABLE_NAME} DESTINATION .)
  file(WRITE DeerPortal.exe.gui "")
  install(FILES DeerPortal.exe.gui DESTINATION .)

ELSE()
	set (ASSETS_BASE_PATH share/games/deerportal)
	install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
ENDIF()

add_definitions(-DBASE_PATH="${CMAKE_INSTALL_PREFIX}")
IF (WIN32)
	add_definitions(-DASSETS_PATH="${ASSETS_BASE_PATH}/")
#install(DIRECTORY assets DESTINATION share/games/deerportal)
install(DIRECTORY assets DESTINATION "${ASSETS_BASE_PATH}/")
ELSE()
	add_definitions(-DASSETS_PATH="${CMAKE_INSTALL_PREFIX}/${ASSETS_BASE_PATH}/")
	# install(DIRECTORY assets DESTINATION share/games/deerportal)
	install(DIRECTORY assets DESTINATION "${CMAKE_INSTALL_PREFIX}/${ASSETS_BASE_PATH}/")

ENDIF()
# install(DIRECTORY assets DESTINATION share/games/deerportal)



# CPack packaging
include(InstallRequiredSystemLibraries)
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${DEERPORTAL_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${DEERPORTAL_VERSION_MINOR}.${DEERPORTAL_VERSION_PATCH}")
include(CPack)
# //target_link_libraries(myproject sfml-graphics sfml-audio)
