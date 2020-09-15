#include <cstdlib>

#include "cppcoro_test.h"
#include "population.h"
#include "simple_generator.h"

int main(int argc, char const *argv[]) {
  frank::coro::simple_generator_test();
  frank::coro::cppcoro_test();
  auto south_population = frank::coro::population(argc, argv);
  if (south_population < frank::coro::EXPECTED_SOUTH_POPULATION)
    std::exit(EXIT_FAILURE);

  std::exit(EXIT_SUCCESS);
}
