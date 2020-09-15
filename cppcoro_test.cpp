#include <cstdint>
#include <iostream>

#include "generator.h"

namespace frank::coro {

cppcoro::generator<const std::uint64_t> fibonacci() {
  std::uint64_t a = 0;
  std::uint64_t b = 1;
  while (true) {
    co_yield b;

    auto tmp = a;
    a = b;
    b += tmp;
  }
}

void cppcoro_test() {
  for (auto i : fibonacci()) {
    if (i > 1'000'000)
      break;

    std::cout << i << '\n';
  }
}

} // namespace frank::coro
