#include "config.h"

#include "find_coroutine.h"

WARNINGS_OFF
#include <iostream>
WARNINGS_ON

#include "simple_generator.h"

namespace frank::coro {

// Copied and adapted from
// ISO/IEC JTC1 SC22 WG21 N4860
// page 209
//
// https://isocpp.org/files/papers/N4860.pdf
// retrieved 2020/09/15

struct generator {
  struct promise_type;

  using handle = std_coro::coroutine_handle<promise_type>;

  struct promise_type {
    static auto get_return_object_on_allocation_failure() {
      return generator{nullptr};
    }

    auto get_return_object() { return generator{handle::from_promise(*this)}; }
    auto initial_suspend() { return std_coro::suspend_always{}; }
    auto final_suspend() noexcept { return std_coro::suspend_always{}; }
    void unhandled_exception() { std::terminate(); }
    void return_void() {}

    auto yield_value(int value) {
      current_value = value;
      return std_coro::suspend_always{};
    }

    int current_value;
  };

  generator(generator const &) = delete;
  generator(generator &&rhs) : coro(rhs.coro) { rhs.coro = nullptr; }

  ~generator() {
    if (coro)
      coro.destroy();
  }

  bool move_next() { return coro ? (coro.resume(), !coro.done()) : false; }
  int current_value() { return coro.promise().current_value; }

private:
  generator(handle h) : coro(h) {}

  handle coro;
};

generator f() {
  co_yield 43;
  co_yield 4687;
}

void simple_generator_test() {
  auto g = f();
  while (g.move_next())
    std::cout << g.current_value() << '\n';
}

} // namespace frank::coro
