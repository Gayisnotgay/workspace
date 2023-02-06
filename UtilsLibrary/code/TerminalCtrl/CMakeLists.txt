cmake_minimum_required(VERSION 3.24)

# 主框架
aux_source_directory(. SRC_LIST)
add_library(TerminalCtrl OBJECT ${SRC_LIST})
install(FILES TerminalCtrl.h DESTINATION include/TerminalCtrl)