cmake_minimum_required(VERSION 2.8.12) # Default version on Ubuntu 14.04

project(glwin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(SPRINGHEAD_DIR   "" CACHE PATH "root directory of Springhead")
set(SCENEBUILDER_DIR "" CACHE PATH "root directory of Scenebuilder")
set(SDL2_DIR         "" CACHE PATH "root directory of SDL2")
set(SDL2_ttf_DIR     "" CACHE PATH "root directory of SDL2_ttf")

if(UNIX)
	set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof -Wno-sign-compare -Wno-delete-non-virtual-dtor -Wno-reorder")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

find_package(GLEW)
find_package(GLUT)
add_definitions(-DFREEGLUT_LIB_PRAGMAS=0)

include_directories(${PROJECT_SOURCE_DIR}/..)
include_directories(${SPRINGHEAD_DIR}/core/include)
include_directories(${SPRINGHEAD_DIR}/core/src)
include_directories(${SCENEBUILDER_DIR}/include)
include_directories(${SDL2_DIR}/include/SDL2)
include_directories(${SDL2_ttf_DIR}/include)
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIR})

set(GLWIN_HEADERS
	glwin.h
    button.h
    camera.h
    console.h
    folder.h
    font.h
    hint.h
    imageviewer.h
    jog.h
    label.h
    listbox.h
    logger.h
    messagebox.h
    outline.h
    radiogroup.h
    rect.h
    scrollbar.h
    switcher.h
    textbox.h
    tree.h
    typedefs.h
    viewer.h
    window.h
    winmanager.h)

set(GLWIN_SOURCES
    button.cpp
    camera.cpp
    console.cpp
    folder.cpp
    font.cpp
    hint.cpp
    imageviewer.cpp
    jog.cpp
    label.cpp
    listbox.cpp
    logger.cpp
    messagebox.cpp
    outline.cpp
    radiogroup.cpp
    scrollbar.cpp
    switcher.cpp
    textbox.cpp
    tree.cpp
    viewer.cpp
    window.cpp
    winmanager.cpp)

add_library(glwin ${GLWIN_SOURCES})

install(TARGETS glwin DESTINATION lib)
install(FILES ${GLWIN_HEADERS} DESTINATION include/glwin)
