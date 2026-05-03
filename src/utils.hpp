#include "raylib-cpp.hpp" // IWYU pragma: export
#include "types.hpp"
using namespace Types;

void DrawEdges(PatternWire edges, int max_edge_x, int max_edge_y, int thickness,
               int width, int height, int start_x, int start_y,
               raylib::Color color);

Point triangleCenter(Triangle t);

PatternWire dualMesh(PatternWire initial_wire);

PatternWire dualMeshWithBoundary(PatternWire initial_wire);

PatternPreFill wireToPolylines(PatternWire wire);
