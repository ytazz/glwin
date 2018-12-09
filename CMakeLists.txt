cmake_minimum_required(VERSION 2.8.12) # Default version on Ubuntu 14.04

project(glwin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(SPRINGHEAD_DIR "" CACHE PATH "root directory of Springhead")
set(SCENEBUILDER_DIR "" CACHE PATH "root directory of Scenebuilder")
set(SDL2_DIR "" CACHE PATH "root directory of SDL2")

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof -Wno-sign-compare -Wno-delete-non-virtual-dtor -Wno-reorder")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(${PROJECT_SOURCE_DIR}/..)
include_directories(${SPRINGHEAD_DIR}/core/include)
include_directories(${SPRINGHEAD_DIR}/core/src)
include_directories(${SCENEBUILDER_DIR}/include)
include_directories(${SDL2_DIR})

add_library(glwin
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
