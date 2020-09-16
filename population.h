#pragma once

#include "config.h"

WARNINGS_OFF
#include <cstdint>
WARNINGS_ON

namespace frank::coro {

using population_count = std::int64_t;

constexpr population_count EXPECTED_SOUTH_POPULATION = 400000000;

extern auto population(int argc, char const *argv[]) -> population_count;

} // namespace frank::coro
