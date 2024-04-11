cmake_minimum_required(VERSION 3.10)

project(zip-viewer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)

# Find minizip-ng library
find_path(MINIZIP_INCLUDE_DIR NAMES minizip-ng/unzip.h)
find_library(MINIZIP_LIBRARY NAMES minizip-ng)

if(NOT MINIZIP_INCLUDE_DIR OR NOT MINIZIP_LIBRARY)
    message(FATAL_ERROR "Could not find minizip-ng library")
endif()

# Set up the executable
add_executable(zip-viewer
    src/main.cpp
    src/zipviewer.cpp
    src/ziputils.cpp
    src/zipmodel.cpp
)

# Set the include directories
target_include_directories(zip-viewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MINIZIP_INCLUDE_DIR}
)

# Disable stack protection
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-stack-protector")

# Link the required libraries
target_link_libraries(zip-viewer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${ZLIB_LIBRARIES}
    ${BZIP2_LIBRARIES}
    ${MINIZIP_LIBRARY}
)

# Set the autogen settings for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)