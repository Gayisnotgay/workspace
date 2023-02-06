cmake_minimum_required(VERSION 3.21)
include(CMakePrintHelpers)
aux_source_directory(. TerminalCtrlFunction-SRC_LIST)
target_sources(${PROJECT_NAME}Functions PRIVATE ${TerminalCtrlFunction-SRC_LIST})
