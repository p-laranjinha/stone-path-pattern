#include "types.hpp"
using namespace Types;

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
