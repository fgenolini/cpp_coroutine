#pragma once

#include "config.h"

WARNINGS_OFF

// Coroutine detection copied and adapted from
// https://github.com/luncliff/coroutine/blob/master/interface/coroutine/channel.hpp
#if __has_include(<coroutine>) && !defined(USE_EXPERIMENTAL_COROUTINE)
#include <coroutine>
namespace std_coro = std;
#elif __has_include(<experimental/coroutine>)
#include <experimental/coroutine>
namespace std_coro = std::experimental;
#endif

WARNINGS_ON
