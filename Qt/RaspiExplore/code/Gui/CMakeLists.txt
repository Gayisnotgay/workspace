cmake_minimum_required(VERSION 3.21)

# 添加工程级头文件包含目录，避免moc时qt自动生成的文件包含不到用户自定义的QObject子类文件
include_directories(${CMAKE_SOURCE_DIR}/code/Gui)

# Qt6相关配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Gui Core Widgets Test)

aux_source_directory(. Gui_SRC_LIST)
list(APPEND Gui_SRC_LIST ./ExploreRaspiAppGui.qrc ./ExploreRaspiAppGui.ui ./GpioGetFunctionWidget.h)
set(PROJECT_GUI_NAME ${PROJECT_NAME}Gui)
qt_add_executable(${PROJECT_GUI_NAME} ${Gui_SRC_LIST})
target_link_libraries(${PROJECT_GUI_NAME} PRIVATE ${PROJECT_NAME}Functions ${PROJECT_LINK_FLAGS} Qt6::Gui Qt6::Core Qt6::Widgets Qt6::Test)
install(TARGETS ${PROJECT_GUI_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/${BIN_OUTPUT_DIR})
