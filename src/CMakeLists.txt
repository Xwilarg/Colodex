file(GLOB_RECURSE SRCS "*.c")
include_directories(../inc/)
add_library(Colodex ${SRCS} ${INCS})
target_link_libraries(Colodex ${CONAN_LIBS})