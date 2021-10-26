include_directories(../inc/colodex)
add_executable(ColodexTest test.cpp)
target_link_libraries(ColodexTest ${CONAN_LIBS} Colodex)