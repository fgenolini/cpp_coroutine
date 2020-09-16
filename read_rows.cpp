// Copied and adapted from the YouTube presentation by ContextFree on
// https://www.youtube.com/watch?v=fkbocd47xDE
// (Demo: C++20 Generator Coroutines)
// Video dated 2020/04/20
#include "config.h"

WARNINGS_OFF
#include <sstream>
WARNINGS_ON

#include "read_rows.h"

namespace frank::coro {

// My own version of split_line, not copied from ContextFree's presentation
static void split_line(std::string const &line,
                       std::vector<std::string> &fields, char delimiter) {
  fields.clear();
  std::stringstream l(line);
  while (l.good()) {
    std::string field{};
    std::getline(l, field, delimiter);
    fields.push_back(field);
  }
}

WARNING_PUSH
DISABLE_WARNING_MSC(4623)
DISABLE_WARNING_MSC(5027)
auto read_rows(std::istream &in, char delimiter)
    -> cppcoro::generator<std::vector<std::string> const &> {
  auto line = std::string{};
  auto columns = std::vector<std::string>{};
  while (std::getline(in, line)) {
    split_line(line, columns, delimiter);
    co_yield columns;
  }
}
WARNINGS_ON

} // namespace frank::coro
