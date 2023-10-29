cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(encoder)

add_executable(encoder
    src/encoder.cpp
    src/fs.cpp
    src/lame_wrapper.cpp
    src/log.cpp
    src/main.cpp
    src/wav.cpp
)

find_package(Lame REQUIRED)
find_package(Sndfile REQUIRED)

list(APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
list(APPEND INCLUDE_DIRS "${LAME_INCLUDE_DIR}")
list(APPEND INCLUDE_DIRS "${SNDFILE_INCLUDE_DIR}")

list(APPEND LIBS "${LAME_LIBRARIES}")
list(APPEND LIBS "${SNDFILE_LIBRARIES}")

set_target_properties(encoder
    PROPERTIES
        INCLUDE_DIRECTORIES "${INCLUDE_DIRS}"
        LINK_LIBRARIES "${LIBS}"
)
