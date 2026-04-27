#include <array>
#include <map>
#include <vector>

namespace types {
using namespace std;

// The coordinates of a vertex.
using vertex = array<float, 2>;
// A collection of ordered by adjacency vertices that make a polygon's wire.
using polyline = vector<vertex>;
// The 2 vertices that make a triangle.
// The vertices should by ordered first by which has the smallest X coordinate,
//  then by which has the smallest Y coordinate.
using edge = array<vertex, 2>;
// The 3 vertices that make a triangle.
using triangle = array<vertex, 3>;

// The calculated center of a polygon, used to ID polygons.
using center = vertex;

// These edges are used to draw the wire, and the center is used to help in
//  implementing the dual mesh process.
using pattern_wire = map<edge, vector<center>>;

// This is a mid-process data structure used to save all vertices belonging to a
//  polygon and their adjacent vertices.
// This should only require being used during the dual mesh process.
using pattern_preprefill = map<center, map<vertex, array<vertex, 2>>>;

// This is a mid-process data structure used to save a polygon's full wire.
// This will be used to triangulate the polygon.
using pattern_prefill = map<center, polyline>;

// This data structure contains a polygon's triangle partitions formed after
//  triangulation, and is used to draw the polygon's area/inside/fill.
using pattern_fill = map<center, vector<triangle>>;

// All that is required to draw a pattern without much more processing.
// The pattern_wire contains all that is required for the dual mesh process.
// When dual meshing, create a new edge from the centers and
//  turn the edge vertices into centers to generate the new wire.
// Then for each new center, save its vertices and their adjacent vertices, turn
//  those into a polygon's wire, then triangulate the polygon to generate the
//  new fill.
using pattern = tuple<pattern_wire, pattern_fill>;

} // namespace types
