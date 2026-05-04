#include "types.hpp"

#include "raygui.h"
#include "raylib-cpp.hpp" // IWYU pragma: export
#include <algorithm>

using namespace std;
using namespace Types;

void DrawEdges(PatternWire edges, int max_edge_x, int max_edge_y, int thickness,
               int width, int height, int start_x, int start_y,
               raylib::Color color) {
  for (const auto &[key, _] : edges) {
    color.DrawLine(raylib::Vector2(key[0][0] * width / max_edge_x + start_x,
                                   key[0][1] * height / max_edge_y + start_y),
                   raylib::Vector2(key[1][0] * width / max_edge_x + start_x,
                                   key[1][1] * height / max_edge_y + start_y),
                   thickness);
  }
}

void DrawFill(PatternFill fill, int max_edge_x, int max_edge_y, int width,
              int height, int start_x, int start_y, raylib::Color color) {
  for (auto [center, triangles] : fill) {
    for (Triangle triangle : triangles) {
      float fade = 0.2 + float(rand()) / RAND_MAX * 0.8;
      DrawTriangle({triangle[0][0] * width / max_edge_x + start_x,
                    triangle[0][1] * height / max_edge_y + start_y},
                   {triangle[1][0] * width / max_edge_x + start_x,
                    triangle[1][1] * height / max_edge_y + start_y},
                   {triangle[2][0] * width / max_edge_x + start_x,
                    triangle[2][1] * height / max_edge_y + start_y},
                   color.Fade(fade));
    }
  }
};

void DrawIntInput(IntInputValue &v, Rectangle bounds, int min, int max) {
  if (GuiValueBox(bounds, "", &get<1>(v), min, max, get<2>(v))) {
    get<2>(v) = !get<2>(v);
    if (!get<2>(v)) {
      get<0>(v) = get<1>(v);
    }
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
    // If an edge only has 1 center, it is an outer edge, and the center is an
    //  outer vertex of the dual mesh.
    // This outer vertex then needs to be connected to the boundary.
    if (centers.size() < 2) {
      Point center = centers[0];

      // We use the edge's center as a vertex for the new connecting edge.
      Point edge_center = {(edge[0][0] + edge[1][0]) / 2,
                           (edge[0][1] + edge[1][1]) / 2};
      new_wire[{center, edge_center}].push_back(edge[0]);
      new_wire[{center, edge_center}].push_back(edge[1]);

      // We generate a new outside edges from the old one.
      new_wire[{edge[0], edge_center}].push_back(edge[0]);
      new_wire[{edge[1], edge_center}].push_back(edge[1]);
      continue;
    }
    Edge new_edge = sortEdge({centers[0], centers[1]});
    for (auto new_center : edge) {
      new_wire[new_edge].push_back(new_center);
    }
  }
  return new_wire;
}

// This should be included in the pattern generation and/or in the dual mesh
// process for efficiency but I'll make it separate and maybe include it in the
// other places later.
PatternPreFill wireToPolylines(PatternWire wire) {
  PatternPrePreFill adjacentVertices;
  for (auto [edge, centers] : wire) {
    for (Point center : centers) {
      adjacentVertices[center][edge[0]].push_back(edge[1]);
      adjacentVertices[center][edge[1]].push_back(edge[0]);
    }
  }
  PatternPreFill polylines;
  for (auto [center, adjacencies] : adjacentVertices) {
    if (adjacencies.size() < 3) {
      continue;
    }
    Polyline polyline;
    // Start the polyline.
    Point start_point = adjacencies.begin()->first;
    if (adjacencies[start_point].size() == 1) {
      // If start_point only has 1 adjacent vertex, it isn't a closed polygon.
      continue;
    }
    polyline.push_back(adjacencies[start_point][0]);
    polyline.push_back(start_point);
    polyline.push_back(adjacencies[start_point][1]);
    for (int i = 0; i < adjacencies.size() - 3; i++) {
      // Getting this here so that it stays the same even after push_back()
      // inside the next for loop.
      Point other_adj = polyline.end()[-2];
      // Get the adjacent vertices to the last vertex added to the polyline.
      for (Point point : adjacencies[polyline.back()]) {
        // If the adjacent vertex isn't in the polyline, add it.
        if (point != other_adj) {
          polyline.push_back(point);
          break;
        }
      }
    }
    // Check if polyline closes.
    for (Point point : adjacencies[polyline.back()]) {
      if (point == polyline[0]) {
        polylines[center] = polyline;
      }
    }
  }
  return polylines;
}

bool isConvex(Point prev, Point point, Point next) {
  float z_cross_product = (next[1] - prev[1]) * (point[0] - prev[0]) -
                          (next[0] - prev[0]) * (point[1] - prev[1]);
  return z_cross_product < 0;
}

bool isInside(Point prev, Point point, Point next, Point new_point) {
  return !isConvex(prev, new_point, point) &&
         !isConvex(point, new_point, next) && !isConvex(next, new_point, prev);
}

bool isEar(Point prev, Point point, Point next, vector<Point> polyline) {
  if (isConvex(prev, point, next)) {
    for (int i = 0; i < polyline.size() - 2; i++) {
      if (polyline[i] != prev && polyline[i] != point && polyline[i] != next) {
        if (isInside(prev, point, next, polyline[i])) {
          return false;
        }
      }
    }
    return true;
  }
  return false;
}

// Using the ear-clipping algorithm because while it isn't the most efficient,
//  it is "easy" to implement and returns decent looking triangulations.
// I'm adapting:
//  https://github.com/ivanfratric/polypartition/blob/b000a4a2a72b46e1305fb6e95b080448d7c12049/src/polypartition.cpp#L429-L519
PatternFill polylinesTriangulation(PatternPreFill polylines) {
  PatternFill pattern_fill;
  for (auto [center, polyline] : polylines) {
    if (polyline.size() < 3) {
      continue;
    }

    // https://stackoverflow.com/a/1165943
    // https://github.com/ivanfratric/polypartition/issues/49
    // I need to revert counter-clockwise polylines because the calculations and
    // the raylib draw depend on the order.
    float is_clockwise_sum = 0;
    for (int i = 0; i < polyline.size(); i++) {
      Point prev = (i != 0) ? polyline[i - 1] : polyline.back();
      Point point = polyline[i];
      is_clockwise_sum += (point[0] - prev[0]) * (point[1] + prev[1]);
    }
    bool is_clockwise = is_clockwise_sum > 0;
    if (!is_clockwise) {
      reverse(polyline.begin(), polyline.end());
    }

    int i = 0;
    // If polyline has 3 points left theres only a triangle left.
    while (polyline.size() > 3) {
      Point prev = (i != 0) ? polyline[i - 1] : polyline.back();
      Point point = polyline[i];
      Point next = (i != polyline.size() - 1) ? polyline[i + 1] : polyline[0];
      if (isEar(prev, point, next, polyline)) {
        pattern_fill[center].push_back({prev, point, next});
        polyline.erase(polyline.begin() + i);
        i = 0;
        continue;
      }
      i++;
    }
    pattern_fill[center].push_back({polyline[0], polyline[1], polyline[2]});
  }
  return pattern_fill;
}
