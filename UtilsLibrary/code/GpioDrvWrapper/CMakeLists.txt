cmake_minimum_required(VERSION 3.24)

# 主框架
aux_source_directory(. SRC_LIST)
add_library(GpioDrvWrapper OBJECT ${SRC_LIST})
target_link_options(GpioDrvWrapper INTERFACE -lgpiod -lgpiodcxx)
install(FILES GpioDrvWrapper.h GpioDrvWrapperGpiod.h DESTINATION include/GpioDrvWrapper)