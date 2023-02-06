cmake_minimum_required(VERSION 3.24)
aux_source_directory(. SRC_LIST)
add_library(GpioConfig OBJECT ${SRC_LIST})
install(FILES GpioConfig.h GpioPeriodConfig.h GpioToneConfig.h DESTINATION include/GpioConfig)