#include "../types.hpp"
#include <map>
#include <vector>

using namespace std;
using namespace types;
map<edge, vector<center>> GridPattern() {
  return {
      {{{{0, 0}, {0, 1}}}, {{0, 1}}},
      {{{{0, 0}, {1, 0}}}, {{0, 1}}},
      {{{{0, 1}, {1, 1}}}, {{0, 1}}},
      {{{{1, 0}, {1, 1}}}, {{0, 1}}},
  };
}
