#pragma once

// Copied and adapted from the YouTube presentation by ContextFree on
// https://www.youtube.com/watch?v=fkbocd47xDE
// (Demo: C++20 Generator Coroutines)
// Video dated 2020/04/20
#include "config.h"

WARNINGS_OFF
#include <array>
#include <sstream>
#include <string>
#include <vector>
WARNINGS_ON

#include "generator.h"

namespace frank::coro {

constexpr auto DEFAULT_DELIMITER = '\t';

// My own version of split_line, not copied from ContextFree's presentation
template <std::size_t FIELD_COUNT>
void split_line(std::string const &line, std::vector<std::string> &fields,
                std::array<int, FIELD_COUNT> const &needed_fields,
                char delimiter = DEFAULT_DELIMITER) {
  fields.clear();
  auto field_index{0};
  std::stringstream l{line};
  std::string field{};
  auto const first_field = needed_fields.begin();
  auto const last_field = needed_fields.end();
  while (std::getline(l, field, delimiter)) {
    if (std::find(first_field, last_field, field_index) != last_field)
      fields.push_back(field);

    ++field_index;
  }
}

WARNING_PUSH
DISABLE_WARNING_MSC(4623)
DISABLE_WARNING_MSC(5027)
template <std::size_t FIELD_COUNT>
auto read_rows(std::istream &in,
               std::array<int, FIELD_COUNT> const &needed_fields,
               char delimiter = DEFAULT_DELIMITER)
    -> cppcoro::generator<std::vector<std::string> const &> {
  auto line = std::string{};
  auto columns = std::vector<std::string>{};
  while (std::getline(in, line)) {
    split_line(line, columns, needed_fields, delimiter);
    co_yield columns;
  }
}
WARNINGS_ON

} // namespace frank::coro
