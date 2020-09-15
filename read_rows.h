#pragma once

#include <string>
#include <vector>

#include "generator.h"

namespace frank::coro {

constexpr auto DEFAULT_DELIMITER = '\t';

extern auto read_rows(std::istream &in, char delimiter = DEFAULT_DELIMITER)
    -> cppcoro::generator<std::vector<std::string> const &>;

} // namespace frank::coro
