#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "generator.h"

namespace frank::coro {

extern cppcoro::generator<const std::vector<std::string> &>
read_rows(std::istream &in);

}
