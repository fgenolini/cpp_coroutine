cmake_minimum_required(VERSION 3.16)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(APP_LOW_NAME cpp_coroutine)

# Replace with the location of your copy of the test data copied and unzipped from 
# http://download.geonames.org/export/dump/cities500.zip
if (WIN32)
  set(TEST_DATA_FILE "D:\\\\tmp\\\\cities500.txt")
else()
  set(TEST_DATA_FILE "/tmp/cities500.txt")
endif()

project(${APP_LOW_NAME} VERSION 1.0.0)

include(CTest)
enable_testing()

# C++ standard required = C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckIncludeFiles)

# Copied and adapted from
# https://github.com/luncliff/coroutine/blob/master/CMakeLists.txt
include(CheckIncludeFileCXX)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)
if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  if(WIN32)
    # Microsoft CLang front-end to MSVC
    check_cxx_compiler_flag("/std:c++latest"            support_latest)
    check_cxx_compiler_flag("/clang:-fcoroutines-ts"    support_coroutine)
    check_include_file_cxx("experimental/coroutine" has_coroutine_ts
      "/std:c++latest")
  else()
    check_cxx_compiler_flag("-std=c++2a"          support_latest)
    check_cxx_compiler_flag("-fcoroutines-ts"     support_coroutine)
    check_include_file_cxx("experimental/coroutine" has_coroutine_ts
      "-std=c++2a")
  endif()

elseif(MSVC)
  # Notice that `/std:c++latest` and `/await` is exclusive to each other.
  # With MSVC, we have to distinguish Coroutines TS & C++ 20 Coroutines
  check_cxx_compiler_flag("/std:c++latest"    support_latest)
  check_cxx_compiler_flag("/await"            support_coroutine)
  check_include_file_cxx("coroutine"  has_coroutine
    "/std:c++latest")

  if(NOT has_coroutine)
    message(STATUS "Try <experimental/coroutine> (Coroutines TS) instead of <coroutine> ...")
      check_include_file_cxx("experimental/coroutine" has_coroutine_ts
        "/std:c++17")
  endif()

  # has coroutine headers?
  if(NOT has_coroutine AND NOT has_coroutine_ts)
    message(FATAL_ERROR "There are no headers for C++ Coroutines")
  endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  # expect GCC 10 or later
  check_cxx_compiler_flag("-std=gnu++20"        support_latest)
  check_cxx_compiler_flag("-fcoroutines"        support_coroutine)
  check_include_file_cxx("coroutine" has_coroutine
    "-std=gnu++20 -fcoroutines")
  set(try_compile_flags "-fcoroutines")
endif()

# support compiler options for coroutine?
if(NOT support_coroutine)
  message(FATAL_ERROR "The compiler doesn't support C++ Coroutines")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(CommonSourcesList
  cppcoro_test.cpp
  population.cpp
  read_rows.cpp
  simple_generator.cpp)

add_executable(${APP_LOW_NAME} ${APP_LOW_NAME}.cpp ${CommonSourcesList})

# Copied and adapted from
# https://github.com/luncliff/coroutine/blob/master/modules/portable/CMakeLists.txt
if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  if(WIN32)
    # 'target_compile_options' removes duplicated -Xclang directive.
    # avoide the removal using cmake flag variable
    target_compile_options(${APP_LOW_NAME}
      PUBLIC
        "/clang:-fcoroutines-ts"
        -fms-compatibility)

    # in this case, we always use portable header
    target_compile_definitions(${APP_LOW_NAME}
      PUBLIC
        USE_PORTABLE_COROUTINE_HANDLE
        _RESUMABLE_FUNCTIONS_SUPPORTED
      PRIVATE FORCE_CLANG)

  elseif(UNIX OR APPLE)
    target_compile_options(${APP_LOW_NAME}
      PUBLIC
        -std=c++2a -stdlib=libc++
        -fcoroutines-ts)
  endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  target_compile_options(${APP_LOW_NAME}
    PUBLIC
      -std=gnu++2a
      -fcoroutines -fconcepts)

  target_link_libraries(${APP_LOW_NAME}
    PUBLIC stdc++)

elseif(MSVC)
  # select between C++ Coroutines TS & C++ 20 Coroutines
  if(support_intrinsic_builtin AND has_coroutine)
    target_compile_options(${APP_LOW_NAME}
      PUBLIC /std:c++latest)
  else()
    target_compile_options(${APP_LOW_NAME}
      PUBLIC /std:c++17 /await)
  endif()
    target_compile_options(${APP_LOW_NAME}
      PRIVATE /JMC-)
endif()

# Set warning level as high as practicable, mute some warnings of less value
target_compile_options(${APP_LOW_NAME} PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>
  $<$<CXX_COMPILER_ID:MSVC>:/D__STDC_WANT_SECURE_LIB__=1 /Wall /w45038 /wd4201 /wd4365 /wd4514 /wd4626 /wd4710 /wd4711 /wd4820>)

target_include_directories(${APP_LOW_NAME} PUBLIC
  "${PROJECT_BINARY_DIR}" "${CMAKE_BINARY_DIR}")

add_test(${APP_LOW_NAME} ${APP_LOW_NAME} ${TEST_DATA_FILE})
