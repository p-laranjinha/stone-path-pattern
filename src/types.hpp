#include <array>
#include <map>
#include <vector>

namespace Types {
using namespace std;

// The coordinates of a point.
using Point = array<float, 2>;
// A collection of ordered by adjacency vertices that make a polygon's wire.
using Polyline = vector<Point>;
// The 2 vertices that make a triangle.
// The vertices should by ordered first by which has the smallest X coordinate,
//  then by which has the smallest Y coordinate.
using Edge = array<Point, 2>;
// The 3 vertices that make a triangle.
using Triangle = array<Point, 3>;

// Hint: map<Edge, vector<CENTER>>.
// These edges are used to draw the wire, and the vector of centers is used to
//  help in implementing the dual mesh process.
using PatternWire = map<Edge, vector<Point>>;

// Hint: map<CENTER, map<VERTEX, array<ADJACENT_VERTEX, 2>>>.
// This is a mid-process data structure used to save all vertices belonging to a
//  polygon and their adjacent vertices.
// This should only require being used during the dual mesh process.
using PatternPrePreFill = map<Point, map<Point, array<Point, 2>>>;

// Hint: map<CENTER, Polyline>.
// This is a mid-process data structure used to save a polygon's full wire.
// This will be used to triangulate the polygon.
using PatternPreFill = map<Point, Polyline>;

// Hint: map<CENTER, vector<Triangle>>.
// This data structure contains a polygon's triangle partitions formed after
//  triangulation, and is used to draw the polygon's area/inside/fill.
using PatternFill = map<Point, vector<Triangle>>;

// All that is required to draw a pattern without much more processing.
// The pattern_wire contains all that is required for the dual mesh process.
// When dual meshing, create a new edge from the centers and
//  turn the edge vertices into centers to generate the new wire.
// Then for each new center, save its vertices and their adjacent vertices, turn
//  those into a polygon's wire, then triangulate the polygon to generate the
//  new fill.
using Pattern = tuple<PatternWire, PatternFill>;

} // namespace Types
