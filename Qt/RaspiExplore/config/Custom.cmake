cmake_minimum_required(VERSION 3.21)

# gpio库相关链接选项
set(PROJECT_LINK_FLAGS -L${CMAKE_SOURCE_DIR}/${LIB_OUTPUT_DIR} -lUtilsLibrary -lgpiod -lgpiodcxx -lwiringPi)
if(NOT CMAKE_CROSSCOMPILING)
    list(PREPEND PROJECT_LINK_FLAGS -Wl,-rpath=${CMAKE_SOURCE_DIR}/${LIB_OUTPUT_DIR})
    cmake_print_variables(PROJECT_LINK_FLAGS)
endif()