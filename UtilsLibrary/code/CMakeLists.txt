cmake_minimum_required(VERSION 3.24)

include_directories(.)
add_subdirectory(MessageCenter)
add_subdirectory(AutoInitRuntimeObject)
add_subdirectory(AutoRegFactory)
add_subdirectory(ParseConfig)
add_subdirectory(GpioDrvWrapper)
add_subdirectory(GpioConfig)
add_subdirectory(NumMscNotationParser)
add_subdirectory(SmallTools)
add_subdirectory(TerminalCtrl)

add_library(${PROJECT_NAME} SHARED dummy.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC MessageCenter AutoInitRuntimeObject ParseConfig GpioDrvWrapper GpioConfig NumMscNotationParser TerminalCtrl)

install(TARGETS ${PROJECT_NAME} DESTINATION ${LIB_OUTPUT_DIR})