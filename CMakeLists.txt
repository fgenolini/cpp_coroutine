cmake_minimum_required(VERSION 3.16)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(APP_LOW_NAME cpp_coroutine)

project(${APP_LOW_NAME} VERSION 1.0.0)

include(CTest)
enable_testing()

# C standard required = C++ 20
# Only GCC 10 and above seem to have coroutine support.
# The others (CLang, MSVC) have the experimental coroutine TS only
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CommonSourcesList
  cppcoro_test.cpp
  population.cpp
  read_rows.cpp
  simple_generator.cpp
  )

add_executable(${APP_LOW_NAME} ${APP_LOW_NAME}.cpp ${CommonSourcesList})
target_compile_options(${APP_LOW_NAME} PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra -fcoroutines>
  $<$<CXX_COMPILER_ID:MSVC>:/Wall /w45038 /wd4201 /wd4365 /wd4626 /wd4710 /wd4711 /wd4820>)

add_test(${APP_LOW_NAME} ${APP_LOW_NAME} "/tmp/cities500.txt")
