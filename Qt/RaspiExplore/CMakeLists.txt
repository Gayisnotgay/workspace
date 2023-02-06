cmake_minimum_required(VERSION 3.21)
project(RaspiExplore LANGUAGES CXX)

#0.0 包含通用构建配置
include(config/Build.cmake)
#0.1 包含自定义构建配置(Qt库)
include(config/Custom.cmake)

#1.0 配置外部工程UtilsLibrary实用工具库
cmake_print_variables(CMAKE_TOOLCHAIN_FILE)
if (CMAKE_TOOLCHAIN_FILE)
    set(TOOLCHAIN_PARA "--toolchain ${CMAKE_TOOLCHAIN_FILE}")
endif()
include(ExternalProject)
ExternalProject_Add(UtilsLibrary 
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../UtilsLibrary 
    CMAKE_ARGS ${TOOLCHAIN_PARA} -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}
    BUILD_ALWAYS 1)

include_directories(include)


#2.0 添加本工程代码目录
add_subdirectory(code)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()