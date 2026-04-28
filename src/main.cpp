#include "patterns/grid.hpp"
#include "patterns/triangle_grid.hpp"
#include "types.hpp"
#include "utils.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib-cpp.hpp" // IWYU pragma: export

using namespace std;
using namespace Types;

int main() {
  // Initialization
  int padding = 5;
  int thickness = 3;
  int count = 10;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  raylib::Window window(800, 800, "stone-path-pattern");
  window.SetTargetFPS(1000);
  GuiLoadStyle("assets/style.rgs");

  // Main game loop
  while (!window.ShouldClose()) { // Detect window close button or ESC key
    if (IsKeyPressed(KEY_S)) {
      TakeScreenshot("screenshot.png");
    }

    while (window.Drawing()) {
      window.ClearBackground(
          raylib::Color(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      int max_x = 10;
      int max_y = 10;
      PatternWire wire = triangleGridPattern(max_x, max_y);
      PatternWire dual = dualMeshWithBoundary(wire);

      DrawEdges(wire, max_x, max_y, thickness, window.GetRenderWidth(),
                window.GetRenderHeight(), padding,
                raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
      DrawEdges(dual, max_x, max_y, thickness, window.GetRenderWidth(),
                window.GetRenderHeight(), padding,
                raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));

      // window.DrawFPS(10, 10);
    }
  }

  return 0;
}
