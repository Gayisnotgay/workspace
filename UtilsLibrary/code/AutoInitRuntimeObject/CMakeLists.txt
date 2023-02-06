cmake_minimum_required(VERSION 3.24)

# 主框架
aux_source_directory(. SRC_LIST)
add_library(AutoInitRuntimeObject OBJECT ${SRC_LIST})
install(FILES AutoInitRuntimeObject.h DESTINATION include/AutoInitRuntimeObject)