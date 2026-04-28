#include "../types.hpp"

using namespace std;
using namespace Types;
PatternWire gridPattern(int max_x, int max_y) {
  PatternWire wire;
  for (float x = 0; x < max_x; x++) {
    for (float y = 0; y < max_y; y++) {
      vector<Edge> edges = {
          {{{x, y}, {x + 1, y}}},         //
          {{{x, y}, {x, y + 1}}},         //
          {{{x + 1, y}, {x + 1, y + 1}}}, //
          {{{x, y + 1}, {x + 1, y + 1}}}, //
      };
      Point center = {x + 0.5f, y + 0.5f};
      for (auto edge : edges) {
        wire[edge].push_back(center);
      }
    };
  };
  return wire;
}
