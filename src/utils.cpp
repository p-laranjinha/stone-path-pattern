#include "types.hpp"

#include "raygui.h"
#include "raylib-cpp.hpp" // IWYU pragma: export

using namespace std;
using namespace Types;

void DrawEdges(PatternWire edges, int max_edge_x, int max_edge_y, int thickness,
               int screen_width, int screen_height, int padding) {
  screen_width = screen_width - padding * 2;
  screen_height = screen_height - padding * 2;
  for (const auto &[key, _] : edges) {
    raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))
        .DrawLine(
            raylib::Vector2(key[0][0] * screen_width / max_edge_x + padding,
                            key[0][1] * screen_height / max_edge_y + padding),
            raylib::Vector2(key[1][0] * screen_width / max_edge_y + padding,
                            key[1][1] * screen_height / max_edge_y + padding),
            thickness);
  }
}

Edge sortEdge(Edge edge) {
  if (edge[0][0] < edge[1][0]) {
    return edge;
  }
  if (edge[0][0] > edge[1][0]) {
    return {edge[1], edge[0]};
  }
  if (edge[0][1] < edge[1][1]) {
    return edge;
  }
  return {edge[1], edge[0]};
}

PatternWire dualMesh(PatternWire initial_wire) {
  PatternWire new_wire;
  for (auto [edge, centers] : initial_wire) {
    if (centers.size() < 2) {
      continue;
    }
    Edge new_edge = sortEdge({centers[0], centers[1]});
    for (auto new_center : edge) {
      new_wire[new_edge].push_back(new_center);
    }
  }
  return new_wire;
}
