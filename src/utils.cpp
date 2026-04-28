#include "types.hpp"

#include "raygui.h"
#include "raylib-cpp.hpp" // IWYU pragma: export

using namespace std;
using namespace Types;

void DrawEdges(PatternWire edges, int max_edge_x, int max_edge_y, int thickness,
               int screen_width, int screen_height, int padding,
               raylib::Color color) {
  screen_width = screen_width - padding * 2;
  screen_height = screen_height - padding * 2;
  for (const auto &[key, _] : edges) {
    color.DrawLine(
        raylib::Vector2(key[0][0] * screen_width / max_edge_x + padding,
                        key[0][1] * screen_height / max_edge_y + padding),
        raylib::Vector2(key[1][0] * screen_width / max_edge_x + padding,
                        key[1][1] * screen_height / max_edge_y + padding),
        thickness);
  }
}

Point triangleCenter(Triangle t) {
  return {(t[0][0] + t[1][0] + t[2][0]) / 3, (t[0][1] + t[1][1] + t[2][1]) / 3};
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

PatternWire dualMeshWithBoundary(PatternWire initial_wire) {
  PatternWire new_wire;
  for (auto [edge, centers] : initial_wire) {
    if (centers.size() < 2) {
      // If an edge only has 1 center, it is an outer edge, and the center is an
      //  outer vertex of the dual mesh.
      // This outer vertex then needs to be connected to the boundary.
      Point center = centers[0];
      Point edge_center = {(edge[0][0] + edge[1][0]) / 2,
                           (edge[0][1] + edge[1][1]) / 2};
      new_wire[{center, edge_center}].push_back(edge[0]);
      new_wire[{center, edge_center}].push_back(edge[1]);
      new_wire[edge].push_back(edge_center);
      continue;
    }
    Edge new_edge = sortEdge({centers[0], centers[1]});
    for (auto new_center : edge) {
      new_wire[new_edge].push_back(new_center);
    }
  }
  return new_wire;
}
