#pragma once

#include <cstdint>

namespace frank::coro {

using population_count = std::int64_t;

constexpr population_count EXPECTED_SOUTH_POPULATION = 422683723;

extern auto population(int argc, char const *argv[]) -> population_count;

} // namespace frank::coro
