#include "../types.hpp"
#include "../utils.hpp"

using namespace std;
using namespace Types;
PatternWire alternatingTriangleGridPattern2(int max_x, int max_y) {
  PatternWire wire;
  for (float x = 0; x < max_x; x++) {
    for (float y = 0; y < max_y; y++) {
      if ((int(x) % 2 == 0 && int(y) % 2 == 0) ||
          (int(x) % 2 != 0 && int(y) % 2 != 0)) {
        vector<Edge> edges1 = {
            {{{x, y}, {x + 1, y}}},     //
            {{{x, y}, {x, y + 1}}},     //
            {{{x, y + 1}, {x + 1, y}}}, //
        };
        vector<Edge> edges2 = {
            {{{x + 1, y}, {x + 1, y + 1}}}, //
            {{{x, y + 1}, {x + 1, y + 1}}}, //
            {{{x, y + 1}, {x + 1, y}}},     //
        };
        Point center1 = triangleCenter({{{x, y}, {x + 1, y}, {x, y + 1}}});
        Point center2 =
            triangleCenter({{{x + 1, y + 1}, {x + 1, y}, {x, y + 1}}});
        for (auto edge : edges1) {
          wire[edge].push_back(center1);
        }
        for (auto edge : edges2) {
          wire[edge].push_back(center2);
        }
      } else {
        vector<Edge> edges1 = {
            {{{x, y}, {x + 1, y}}},         //
            {{{x + 1, y}, {x + 1, y + 1}}}, //
            {{{x, y}, {x + 1, y + 1}}},     //
        };
        vector<Edge> edges2 = {
            {{{x, y}, {x, y + 1}}},         //
            {{{x, y + 1}, {x + 1, y + 1}}}, //
            {{{x, y}, {x + 1, y + 1}}},     //
        };
        Point center1 = triangleCenter({{{x, y}, {x + 1, y}, {x + 1, y + 1}}});
        Point center2 = triangleCenter({{{x, y}, {x, y + 1}, {x + 1, y + 1}}});
        for (auto edge : edges1) {
          wire[edge].push_back(center1);
        }
        for (auto edge : edges2) {
          wire[edge].push_back(center2);
        }
      }
    }
  }
  return wire;
}
