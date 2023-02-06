cmake_minimum_required(VERSION 3.21)
include(CMakePrintHelpers)

# 设置C++标准
if((NOT CMAKE_CXX_STANDARD) OR (CMAKE_CXX_STANDARD STREQUAL ""))
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    cmake_print_variables(CMAKE_CXX_STANDARD)
endif()

## 设置编译选项
# add_compile_options(-O0 -Wall -Werror -Wno-psabi -fPIC -fsanitize=address -fno-omit-frame-pointer -fsanitize-recover=address )
# add_link_options(-fsanitize=address -lasan -latomic)
add_compile_options(-Wall -Werror -Wno-psabi -fPIC)
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    add_compile_options(-g)
endif()

# 配置测试框架
include(GoogleTest)
find_package(GTest 1.8 REQUIRED CONFIG)
cmake_print_variables(GTest_FOUND)
get_target_property(GTest_INCLUDE_DIR GTest::gtest INTERFACE_INCLUDE_DIRECTORIES)
cmake_print_variables(GTest_INCLUDE_DIR)
enable_testing()


# 配置二进制目录
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX  ${CMAKE_SOURCE_DIR} CACHE PATH "..." FORCE)
endif()

set(BIN_OUTPUT_DIR bin_${CMAKE_SYSTEM_PROCESSOR})
set(LIB_OUTPUT_DIR lib_${CMAKE_SYSTEM_PROCESSOR})
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${BIN_OUTPUT_DIR})
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${LIB_OUTPUT_DIR})