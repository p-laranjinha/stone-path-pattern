#include "raylib-cpp.hpp" // IWYU pragma: export
#include <iostream>

int main() {
  // Initialization
  int initial_screen_width = 800;
  int initial_screen_height = initial_screen_width;
  int padding = 10;
  int thickness = 3;
  int count = 10;

  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  raylib::Window window(initial_screen_width, initial_screen_height,
                        "stone-path-pattern");
  window.SetTargetFPS(120);

  raylib::Color foreground = raylib::Color::Black();

  // Main game loop
  while (!window.ShouldClose()) { // Detect window close button or ESC key
    if (raylib::Keyboard::IsKeyPressed(KEY_S)) {
      raylib::TakeScreenshot("screenshot.png");
    }
    while (window.Drawing()) {
      window.ClearBackground(raylib::Color::RayWhite());

      int screen_width = window.GetWidth();
      int screen_height = window.GetHeight();
      int square_width = (screen_width - padding * 2) / count;
      int square_height = (screen_height - padding * 2) / count;

      for (int i = 0; i < count; i++) {
        raylib::Vector2 start(padding + square_width * i,
                              padding + square_height * i);
        raylib::Vector2 end(padding + square_width * (i + 1),
                            padding + square_height * i);
        foreground.DrawLine(start, end, thickness);
      }
    }
  }

  return 0;
}
