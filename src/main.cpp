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

  Camera camera = {0};
  camera.position = (Vector3){0.0f, 3.0f, 3.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  raylib::Model model = LoadModelFromMesh(GenMeshPlane(1, 1, 1, 1));

  // Main game loop
  while (!window.ShouldClose()) { // Detect window close button or ESC key
    if (raylib::Keyboard::IsKeyPressed(KEY_S)) {
      raylib::TakeScreenshot("screenshot.png");
    }
    while (window.Drawing()) {
      window.ClearBackground(raylib::Color::RayWhite());

      BeginMode3D(camera);
      DrawModelWiresEx(model, (Vector3){0.0f, 0.0f, 0.0f},
                       (Vector3){0.5f, 1.0f, 0.0f}, 0.0f,
                       (Vector3){1.0f, 1.0f, 1.0f}, DARKGRAY);
      DrawGrid(10, 1.0f);
      EndMode3D();
    }
  }

  return 0;
}
