#include "types.hpp"
using namespace Types;

void DrawEdges(PatternWire edges, int max_edge_x, int max_edge_y, int thickness,
               int screen_width, int screen_height, int padding);

Point triangleCenter(Triangle t);

PatternWire dualMesh(PatternWire initial_wire);
