cmake_minimum_required(VERSION 3.16)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(APP_LOW_NAME cpp_coroutine)

# Replace with the location of your copy of the test data copied and unzipped from 
# http://download.geonames.org/export/dump/cities500.zip
set(TEST_DATA_FILE "/tmp/cities500.txt")

project(${APP_LOW_NAME} VERSION 1.0.0)

include(CTest)
enable_testing()

# C++ standard required = C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckIncludeFiles)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(CommonSourcesList
  cppcoro_test.cpp
  population.cpp
  read_rows.cpp
  simple_generator.cpp
  )

add_executable(${APP_LOW_NAME} ${APP_LOW_NAME}.cpp ${CommonSourcesList})

target_compile_options(${APP_LOW_NAME} PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -fcoroutines>
  $<$<CXX_COMPILER_ID:MSVC>:/D__STDC_WANT_SECURE_LIB__=1 /Wall /w45038 /wd4201 /wd4365 /wd4514 /wd4626 /wd4710 /wd4711 /wd4820>)

target_include_directories(${APP_LOW_NAME} PUBLIC
  "${PROJECT_BINARY_DIR}" "${CMAKE_BINARY_DIR}")

add_test(${APP_LOW_NAME} ${APP_LOW_NAME} ${TEST_DATA_FILE})
