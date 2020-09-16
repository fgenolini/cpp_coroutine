// Copied and adapted from the YouTube presentation by ContextFree on
// https://www.youtube.com/watch?v=fkbocd47xDE
// (Demo: C++20 Generator Coroutines)
// Video dated 2020/04/20

#include "config.h"

WARNINGS_OFF
#include <fstream>
#include <iostream>
#include <string>
WARNINGS_ON

#include "population.h"
#include "read_rows.h"

namespace frank::coro {

// Expect argv[1] to be a copy of the text file obtained by unzipping
// http://download.geonames.org/export/dump/cities500.zip
// into cities500.txt
auto population(int argc, char const *argv[]) -> population_count {
  constexpr auto ARGUMENT_1 = 1;
  constexpr auto FIELD_LATITUDE = 4;
  constexpr auto FIELD_POPULATION = 14;

  if (argc <= ARGUMENT_1 || !argv)
    return 0;

  auto args = std::vector<char const *>(argv, argv + argc);
  std::string file_name{args.at(ARGUMENT_1)};
  auto file = std::ifstream{file_name};
  auto population_south = population_count{0};
  auto population_total = population_count{0};
  auto row_count = 0;
  for (auto &row : read_rows(file)) {
    ++row_count;
    if (row.size() <= FIELD_LATITUDE)
      continue;

    auto latitude_text = row.at(FIELD_LATITUDE);
    if (row.size() <= FIELD_POPULATION)
      continue;

    auto population_text = row.at(FIELD_POPULATION);
    auto latitude = (population_count)std::stod(latitude_text);
    auto population = (population_count)std::stod(population_text);
    population_total += population;
    if (latitude < 0)
      population_south += population;
  }

  std::cout << "South population: " << population_south << '\n';
  std::cout << "Total population: " << population_total << '\n';
  std::cout << "            Rows: " << row_count << '\n';
  return population_south;
}

} // namespace frank::coro
