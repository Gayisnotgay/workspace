cmake_minimum_required(VERSION 3.21)
aux_source_directory(. code-SRC_LIST)
add_executable(${PROJECT_NAME} ${code-SRC_LIST})

add_library(${PROJECT_NAME}Functions SHARED)
add_subdirectory(GpioFunctions)
add_subdirectory(TerminalCtrlFunction)
target_include_directories(${PROJECT_NAME}Functions INTERFACE .)
add_custom_command(TARGET ${PROJECT_NAME}Functions POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}Functions> ${CMAKE_SOURCE_DIR}/${LIB_OUTPUT_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LINK_FLAGS} ${PROJECT_NAME}Functions)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/${BIN_OUTPUT_DIR})