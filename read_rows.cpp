// Copied and adapted from YouTube presentation by ContextFree on
// https://www.youtube.com/watch?v=fkbocd47xDE
// Video dated 2020/04/20
// titled: Demo: C++20 Generator Coroutines

#include <sstream>
#include <string>

#include "read_rows.h"

namespace frank::coro {

void split_into(std::string_view const line, std::vector<std::string> &fields) {
  constexpr auto DELIMITER = '\t';

  fields.clear();
  std::stringstream l(std::string{line});
  while (l.good()) {
    std::string field{};
    std::getline(l, field, DELIMITER);
    fields.push_back(field);
  }
}

auto read_rows(std::istream &in)
    -> cppcoro::generator<const std::vector<std::string> &> {
  auto line = std::string{};
  auto columns = std::vector<std::string>{};
  while (std::getline(in, line)) {
    split_into(std::string_view{line}, columns);
    co_yield columns;
  }
}

} // namespace frank::coro
