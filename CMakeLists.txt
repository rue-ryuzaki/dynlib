include(cmake/prelude.cmake)

if (${CMAKE_VERSION} VERSION_LESS "3.12")
    cmake_minimum_required(VERSION 3.0)
else()
    cmake_minimum_required(VERSION 3.0...3.12)
endif()

project("dynlib"
    LANGUAGES CXX
)

set(PROJECT_HOMEPAGE_URL "https://github.com/rue-ryuzaki/dynlib")

include(cmake/project-is-top-level.cmake)
include(cmake/warnings.cmake)

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBRARY_NAME ${PROJECT_NAME})

message(STATUS "${LIBRARY_NAME} version: ${PROJECT_VERSION}")
message(STATUS "${LIBRARY_NAME} build standard: C++${CMAKE_CXX_STANDARD}")

file(GLOB LIBRARY_SOURCES "src/*.h*" "src/*.c*")

add_library(${LIBRARY_NAME} ${LIBRARY_SOURCES})
