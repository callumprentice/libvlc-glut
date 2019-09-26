################################################################################
cmake_minimum_required(VERSION 3.7)

################################################################################
project(libvlc)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -W4 -wd4100 -wd4505")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -W4 -wd4100 -wd4505")

add_executable(
    libvlc-glut
    src/libvlc-glut.cpp
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT libvlc-glut)

target_include_directories(
    libvlc-glut
    PUBLIC
    src
    freeglut/include/GL
    ${VLC_DIR}/sdk/include
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(FREEGLUT_LIBRARY "${PROJECT_SOURCE_DIR}/freeglut/lib/x64/freeglut.lib")
    set(FREEGLUT_DLL "${PROJECT_SOURCE_DIR}/freeglut/bin/x64/freeglut.dll")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(FREEGLUT_LIBRARY "${PROJECT_SOURCE_DIR}/freeglut/lib/freeglut.lib")
    set(FREEGLUT_DLL "${PROJECT_SOURCE_DIR}/freeglut/bin/freeglut.dll")
endif()

set(VLC_DLL "${VLC_DIR}/libvlc.dll")
set(VLCCORE_DLL "${VLC_DIR}/libvlccore.dll")

find_library(
    VLC_LIBRARY_DEBUG
    NAMES libvlc.lib
    PATHS ${VLC_DIR}/sdk/lib
    PATH_SUFFIXES debug
)
find_library(
    VLCCORE_LIBRARY_DEBUG
    NAMES libvlccore.lib
    PATHS ${VLC_DIR}/sdk/lib
    PATH_SUFFIXES debug
)
find_library(
    VLC_LIBRARY_RELEASE
    NAMES libvlc.lib
    PATHS ${VLC_DIR}/sdk/lib
    PATH_SUFFIXES release
)
find_library(
    VLCCORE_LIBRARY_RELEASE
    NAMES libvlccore.lib
    PATHS ${VLC_DIR}/sdk/lib
    PATH_SUFFIXES release
)

set(VLC_LIBRARY
    debug ${VLC_LIBRARY_DEBUG}
    optimized ${VLC_LIBRARY_RELEASE}
)
set(VLCCORE_LIBRARY
    debug ${VLCCORE_LIBRARY_DEBUG}
    optimized ${VLCCORE_LIBRARY_RELEASE}
)

target_link_libraries(
    libvlc-glut
    ${FREEGLUT_LIBRARY}
    ${VLC_LIBRARY}
    ${VLCCORE_LIBRARY}
)

add_custom_command(
    TARGET libvlc-glut POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
            "${FREEGLUT_DLL}"
            "$<TARGET_FILE_DIR:libvlc-glut>"
    COMMENT "Copying FreeGLUT DLL to executable directory"
)

add_custom_command(
    TARGET libvlc-glut POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
            "${VLC_DLL}"
            "$<TARGET_FILE_DIR:libvlc-glut>"
    COMMENT "Copying VLC DLL to executable directory"
)

add_custom_command(
    TARGET libvlc-glut POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
            "${VLCCORE_DLL}"
            "$<TARGET_FILE_DIR:libvlc-glut>"
    COMMENT "Copying VLC (Core) DLL to executable directory"
)

add_custom_command(
    TARGET libvlc-glut POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            ${VLC_DIR}/plugins
            "$<TARGET_FILE_DIR:libvlc-glut>/plugins"
    COMMENT "Copying VLC plugins to executable directory"
)

set_target_properties(libvlc-glut PROPERTIES LINK_FLAGS "/ignore:4099")

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
endif()
