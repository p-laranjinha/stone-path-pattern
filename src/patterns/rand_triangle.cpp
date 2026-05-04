#include "../types.hpp"
#include "../utils.hpp"
#include <raylib.h>

using namespace std;
using namespace Types;
PatternWire randTrianglePattern(int max_x, int max_y, float diagonal_chance,
                                float right_diagonal_chance) {
  PatternWire wire;
  for (float x = 0; x < max_x; x++) {
    for (float y = 0; y < max_y; y++) {
      float percentage = float(rand()) / RAND_MAX;
      if (percentage < right_diagonal_chance * diagonal_chance) {
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
      } else if (percentage < diagonal_chance) {
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
      } else {
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
      }
    }
  }
  return wire;
}
