#include "raylib-cpp.hpp" // IWYU pragma: export
#include "types.hpp"
using namespace Types;

void DrawEdges(PatternWire edges, int max_edge_x, int max_edge_y, int thickness,
               int width, int height, int start_x, int start_y,
               raylib::Color color);

void DrawFill(PatternFill fill, int max_edge_x, int max_edge_y, int width,
              int height, int start_x, int start_y, raylib::Color color,
              raylib::Color hover_color);

void DrawIntInput(IntInputValue &v, Rectangle bounds, int min, int max);

Point triangleCenter(Triangle t);

PatternWire dualMesh(PatternWire initial_wire);

PatternWire dualMeshWithBoundary(PatternWire initial_wire, float max_offset,
                                 int max_x, int max_y);

PatternPreFill wireToPolylines(PatternWire wire);

PatternFill polylinesTriangulation(PatternPreFill polylines);

void randomizeCenterPositions(PatternWire &wire, float max_offset);
