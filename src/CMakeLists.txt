cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(tracer VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/modules")

enable_testing()
add_subdirectory(libs/googletest EXCLUDE_FROM_ALL)
add_subdirectory(libtracer)
add_subdirectory(app)
