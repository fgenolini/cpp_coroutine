// Copied and adapted from YouTube presentation by ContextFree on
// https://www.youtube.com/watch?v=fkbocd47xDE
// Video dated 2020/04/20
// titled: Demo: C++20 Generator Coroutines

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "read_rows.h"

namespace frank::coro {

// Expect argument 1 (second argument) to be a copy of the text file
// obtained by unzipping
// http://download.geonames.org/export/dump/cities500.zip
// into cities500.txt
void population(int argc, char *argv[]) {
  using Population = std::int64_t;
  auto args = std::vector<char *>(argv, argv + argc);
  std::string file_name{args.at(1)};
  std::cerr << "File: " << file_name << '\n';
  auto file = std::ifstream{file_name};

  auto population_south = Population{0};
  auto population_total = Population{0};
  auto row_count = 0;
  for (auto &row : read_rows(file)) {
    ++row_count;
    if (row.size() < 4)
      continue;

    auto latitude_text = row.at(4);
    if (row.size() < 14)
      continue;

    auto population_text = row.at(14);
    auto latitude = std::stod(std::string(latitude_text));
    auto population = std::stod(std::string(population_text));
    population_total += population;
    if (latitude < 0)
      population_south += population;
  }

  std::cout << "South population: " << population_south << '\n';
  std::cout << "Total population: " << population_total << '\n';
  std::cout << "            Rows: " << row_count << '\n';
}

} // namespace frank::coro
