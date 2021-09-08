cmake_minimum_required(VERSION 2.8)
project(Colodex)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(MSVC)
    SET(CMAKE_C_FLAGS "/W4 /EHsc")
else()
    SET(CMAKE_C_FLAGS "-Wall -Wextra -L.")
endif()
file(GLOB_RECURSE SRCS "src/*.c")
include_directories(inc)
add_library(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

add_executable(ColodexTest test/test.c)
target_link_libraries(ColodexTest ${CONAN_LIBS} ${PROJECT_NAME})