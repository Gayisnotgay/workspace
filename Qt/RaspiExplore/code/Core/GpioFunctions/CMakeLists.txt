cmake_minimum_required(VERSION 3.21)
include(CMakePrintHelpers)
aux_source_directory(. GpioFunctions-SRC_LIST)
target_sources(${PROJECT_NAME}Functions PRIVATE ${GpioFunctions-SRC_LIST})
