#include "raylib-cpp.hpp" // IWYU pragma: export
#include <iostream>

int main() {
  // Initialization
  int screenWidth = 800;
  int screenHeight = 450;
  int fontSize = 20;
  int spacing = 2;
  std::string text = "Press 's' to take a screenshot or 'esc' to exit.";

  raylib::Window window(screenWidth, screenHeight,
                        "raylib [core] example - basic window");

  raylib::Font font = GetFontDefault();
  raylib::Vector2 fontPosition(
      screenWidth / 2.0f -
          MeasureTextEx(font, text.c_str(), fontSize, spacing).x / 2,
      screenHeight / 2.0f - fontSize / 2.0f);

  SetTargetFPS(60);

  // Main game loop
  while (!window.ShouldClose()) { // Detect window close button or ESC key
    if (raylib::Keyboard::IsKeyPressed(KEY_S)) {
      raylib::TakeScreenshot("screenshot.png");
    }
    while (window.Drawing()) {
      window.ClearBackground(raylib::Color::RayWhite());
      font.DrawText(text, fontPosition, fontSize, spacing,
                    raylib::Color::LightGray());
    }
  }

  return 0;
}
