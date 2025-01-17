cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(geode-ipc VERSION 0.1.0 LANGUAGES C CXX)

add_library(${PROJECT_NAME} STATIC src/geode-ipc.c src/geode-ipc.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include/)

include(CPM.cmake)
CPMAddPackage("gh:geode-sdk/json@3.2.0")

target_link_libraries(${PROJECT_NAME} PUBLIC mat-json)

if(APPLE)
	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Foundation")
endif()


if (MAKE_TEST)
	add_executable(${PROJECT_NAME}-test test/test.cpp)
	target_link_libraries(${PROJECT_NAME}-test PRIVATE ${PROJECT_NAME})
endif()