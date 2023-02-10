cmake_minimum_required(VERSION 3.21)
project(DeerPortal LANGUAGES CXX)

set(BUILD_SHARED_LIBS FALSE)
# To unify version handling
file (STRINGS "version.txt" DEERPORTAL_VERSION)
string(REGEX MATCH "^([0-9]+)" DEERPORTAL_VERSION_MAJOR ${DEERPORTAL_VERSION})
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+).*" "\\2" DEERPORTAL_VERSION_MINOR ${DEERPORTAL_VERSION})
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+.*)" "\\3" DEERPORTAL_VERSION_PATCH ${DEERPORTAL_VERSION})
MESSAGE( STATUS "DEERPORTAL_VERSION:         " ${DEERPORTAL_VERSION} )
MESSAGE( STATUS "DEERPORTAL_VERSION_MAJOR:         " ${DEERPORTAL_VERSION_MAJOR} )
MESSAGE( STATUS "DEERPORTAL_VERSION_MINOR:         " ${DEERPORTAL_VERSION_MINOR} )
MESSAGE( STATUS "DEERPORTAL_VERSION_PATCH:         " ${DEERPORTAL_VERSION_PATCH} )
add_definitions(-DDEERPORTAL_VERSION="${DEERPORTAL_VERSION}.${CMAKE_BUILD_TYPE}")
add_definitions(-DBASE_PATH="${CMAKE_INSTALL_PREFIX}")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG feature/prep_next_release)
FetchContent_MakeAvailable(SFML)

# add_executable(CMakeSFMLProject src/main.cpp)
set(EXECUTABLE_NAME "DeerPortal")
file( GLOB SRCS src/*.cpp src/*.h )
add_executable(${EXECUTABLE_NAME}  ${SRCS})
target_link_libraries(DeerPortal PRIVATE sfml-graphics sfml-audio sfml-system)
target_compile_features(DeerPortal PRIVATE cxx_std_17)

# target_compile_features(DeerPortal PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET DeerPortal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:DeerPortal> $<TARGET_FILE_DIR:DeerPortal> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS DeerPortal)		
