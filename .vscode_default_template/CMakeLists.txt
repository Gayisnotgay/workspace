cmake_minimum_required(VERSION 3.24)
project(HelloWorld LANGUAGES CXX)
include(config/build.cmake)
include(config/custom.cmake)


if (IsSupportOpenMPAndSantilizer)
    add_executable(${PROJECT_NAME} code/TestOpenMPandSantilizer.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
    target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_FLAGS_NEEDS_TO_CHECK})
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address -lasan)
else()
message(STATUS "TestOpenMPandSantilizer failed, IsSupportOpenMPAndSantilizer: " ${IsSupportOpenMPAndSantilizer})
endif()

# set(COMPILE_FLAGS_NEEDS_TO_CHECK fopenmp)
# list(APPEND COMPILE_FLAGS_NEEDS_TO_CHECK -fsantilizer=address -fsanitize=address -fno-omit-frame-pointer -fsanitize-recover=address)
