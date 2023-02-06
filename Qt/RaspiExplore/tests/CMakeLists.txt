cmake_minimum_required(VERSION 3.24)

add_executable(${PROJECT_NAME}_test)
aux_source_directory(. tests-SRC_LIST)
# 目标板上才运行Testlibgpiod.cpp，里面会访问GPIO硬件
if(NOT CMAKE_CROSSCOMPILING)
    list(REMOVE_ITEM tests-SRC_LIST ./Testlibgpiod.cpp)
    cmake_print_variables(tests-SRC_LIST)
endif()
target_sources(${PROJECT_NAME}_test PRIVATE ${tests-SRC_LIST})
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME}Functions ${PROJECT_LINK_FLAGS} GTest::gtest GTest::gtest_main GTest::gmock)
add_custom_command(TARGET ${PROJECT_NAME}_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}_test> ${CMAKE_SOURCE_DIR}/${BIN_OUTPUT_DIR})
# gtest_discover_tests cannot be used in crosscompiling environment, so we choose gtest_add_tests which scan the sources.
# gtest_discover_tests(${PROJECT_NAME}_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/${BIN_OUTPUT_DIR})
gtest_add_tests(TARGET ${PROJECT_NAME}_test SOURCES ${tests-SRC_LIST} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/${BIN_OUTPUT_DIR})