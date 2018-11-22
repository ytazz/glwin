cmake_minimum_required(VERSION 2.8.12) # Default version on Ubuntu 14.04

project(glwin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

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
