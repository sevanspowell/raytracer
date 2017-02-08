################################# Fever ########################################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(libtracer VERSION 0.0.1 LANGUAGES C CXX)

add_library(tracer

  )

target_include_directories(tracer
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE
  src
  )

set_property(TARGET tracer PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET tracer PROPERTY CXX_STANDARD 11)

# target_compile_features(tracer
#   )

target_link_libraries(tracer
  )

install(TARGETS tracer EXPORT tracerConfig
  ARCHIVE DESTINATION lib 
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  )
install(DIRECTORY include/ DESTINATION include)

# Make project importable from the install directory
install(EXPORT tracerConfig DESTINATION share/tracer/cmake)

# Make project importable from the build directory
export(TARGETS tracer FILE tracerConfig.cmake)

################################# Unit tests ###################################
add_executable(tracerTest
  test/test.cpp
  )

target_link_libraries(tracerTest
  gtest
  tracer
  )

add_test(tracerTest tracerTest)
