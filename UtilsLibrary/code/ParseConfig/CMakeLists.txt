cmake_minimum_required(VERSION 3.24)
aux_source_directory(. SRC_LIST)
add_library(ParseConfig OBJECT ${SRC_LIST})
install(FILES ParseConfig.h DESTINATION include/ParseConfig)