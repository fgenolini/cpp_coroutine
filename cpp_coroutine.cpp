#include "cppcoro_test.h"
#include "population.h"
#include "simple_generator.h"

int main(int argc, char *argv[]) {
  frank::coro::simple_generator_test();
  frank::coro::cppcoro_test();
  frank::coro::population(argc, argv);
  return 0;
}
