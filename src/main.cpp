#include "raylib-cpp.hpp" // IWYU pragma: export
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <iostream>

raylib::Color GetStyleColor(GuiDefaultProperty prop) {
  return GetColor(GuiGetStyle(DEFAULT, prop));
}

int main() {
  // Initialization
  int initial_screen_width = 800;
  int initial_screen_height = 450;
  int padding = 10;
  int thickness = 3;
  int count = 10;

  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  raylib::Window window(initial_screen_width, initial_screen_height,
                        "stone-path-pattern");
  window.SetTargetFPS(120);
  GuiLoadStyle("assets/style.rgs");

  // Main game loop
  while (!window.ShouldClose()) { // Detect window close button or ESC key
    if (IsKeyPressed(KEY_S)) {
      TakeScreenshot("screenshot.png");
    }

    while (window.Drawing()) {
      window.ClearBackground(GetStyleColor(BACKGROUND_COLOR));
      window.DrawFPS(10, 10);
    }
  }

  return 0;
}
