cmake_minimum_required(VERSION 2.8.12)
project(Colodex)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(MSVC)
    SET(CMAKE_C_FLAGS "/W4 /EHsc")
    SET(CMAKE_CXX_FLAGS "/W4 /EHsc /utf-8")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
else()
    SET(CMAKE_C_FLAGS "-Wall -Wextra -L. -D_DEFAULT_SOURCE")
    SET(CMAKE_CXX_FLAGS "-Wall -Wextra -L.")
endif()

add_subdirectory(src)

add_subdirectory(test)