cmake_minimum_required(VERSION 3.24)
aux_source_directory(. SRC_LIST)
add_library(MessageCenter OBJECT ${SRC_LIST})
install(FILES MessageCenter.h DESTINATION include/MessageCenter)