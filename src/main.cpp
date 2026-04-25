#include "patterns/grid.hpp"
#include "types.hpp"
#include <iostream>
#include <map>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib-cpp.hpp" // IWYU pragma: export

using namespace std;
using namespace types;

template <typename _>
void DrawEdges(map<edge, _> edges, int max_edge_x, int max_edge_y,
               int thickness, int screen_width, int screen_height,
               int padding) {
  screen_width = screen_width - padding * 2;
  screen_height = screen_height - padding * 2;
  for (const auto &[key, __] : edges) {
    raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL))
        .DrawLine(
            raylib::Vector2(key[0][0] * screen_width / max_edge_x + padding,
                            key[0][1] * screen_height / max_edge_y + padding),
            raylib::Vector2(key[1][0] * screen_width / max_edge_y + padding,
                            key[1][1] * screen_height / max_edge_y + padding),
            thickness);
  }
}

int main() {
  // Initialization
  int padding = 5;
  int thickness = 3;
  int count = 10;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  raylib::Window window(800, 800, "stone-path-pattern");
  window.SetTargetFPS(120);
  GuiLoadStyle("assets/style.rgs");

  // Main game loop
  while (!window.ShouldClose()) { // Detect window close button or ESC key
    if (IsKeyPressed(KEY_S)) {
      TakeScreenshot("screenshot.png");
    }

    while (window.Drawing()) {
      window.ClearBackground(
          raylib::Color(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
      window.DrawFPS(10, 10);
      DrawEdges(GridPattern(), 1, 1, thickness, window.GetRenderWidth(),
                window.GetRenderHeight(), padding);
    }
  }

  return 0;
}
