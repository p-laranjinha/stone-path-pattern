#include <array>

namespace types {
using vertex = std::array<float, 2>;
using center = vertex;
using edge = std::array<types::vertex, 2>;
} // namespace types
