#include "raylib-cpp.hpp" // IWYU pragma: export
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <iostream>

int main() {
  // Initialization
  int initial_screen_width = 800;
  int initial_screen_height = 450;
  int padding = 10;
  int thickness = 3;
  int count = 10;

  // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(initial_screen_width, initial_screen_height, "stone-path-pattern");
  SetTargetFPS(120);

  Vector2 center = {(GetScreenWidth() - 300) / 2.0f, GetScreenHeight() / 2.0f};

  bool drawOutline = false;
  bool drawPoints = true;

  float edgeBlur = 1.0;
  float smoothness = 5.0;

  Vector2 points[256] = {0};
  int numPoints = 10;

  int hotPoint = -1;
  int draggingPoint = -1;

  // Generate some points
  float innerRadius = 120.0f;
  float outerRadius = 180.0f;
  for (int i = 0; i < numPoints; i++) {
    float angle = 2.0f * PI * (float)i / (float)numPoints;
    float radius = innerRadius + (float)(i & 1) * (outerRadius - innerRadius);
    points[i].x = center.x + cosf(angle) * radius;
    points[i].y = center.y + sinf(angle) * radius;
  }

  // Load the shader and get locations of uniforms
  Shader polyShader = LoadShader("src/shader.vs", "src/shader.fs");
  int pointsLoc = GetShaderLocation(polyShader, "points");
  int edgeBlurLoc = GetShaderLocation(polyShader, "edgeBlur");
  int smoothnessLoc = GetShaderLocation(polyShader, "smoothness");

  // Main game loop
  while (!WindowShouldClose()) { // Detect window close button or ESC key
    if (IsKeyPressed(KEY_S)) {
      TakeScreenshot("screenshot.png");
    }

    // Update
    if (draggingPoint != -1) {
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        // Stop dragging
        draggingPoint = -1;
      } else {
        // Move the point to the mouse
        points[draggingPoint] = GetMousePosition();
      }
    } else {
      // Find the hot point, if any
      Vector2 mouse = GetMousePosition();
      hotPoint = -1;
      for (int i = 0; i < numPoints; i++) {
        if (fabs(mouse.x - points[i].x) < 5 &&
            fabs(mouse.y - points[i].y) < 5) {
          hotPoint = i;
        }
      }

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // Start dragging
        draggingPoint = hotPoint;
      }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawLine(500, 0, 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.6f));
    DrawRectangle(500, 0, GetScreenWidth() - 500, GetScreenHeight(),
                  Fade(LIGHTGRAY, 0.3f));

    SetShaderValueV(polyShader, pointsLoc, points, SHADER_UNIFORM_VEC2,
                    numPoints);
    SetShaderValue(polyShader, edgeBlurLoc, &edgeBlur, SHADER_UNIFORM_FLOAT);
    SetShaderValue(polyShader, smoothnessLoc, &smoothness,
                   SHADER_UNIFORM_FLOAT);
    BeginShaderMode(polyShader);
    DrawRectangle(0, 0, 500, 500, GREEN);
    EndShaderMode();

    // Draw points
    if (drawPoints) {
      for (int i = 0; i < numPoints; i++) {
        DrawCircle(points[i].x, points[i].y,
                   (i == hotPoint || i == draggingPoint) ? 5 : 3, WHITE);
      }
    }

    // Draw basic outline
    if (drawOutline) {
      for (int i = 0; i < numPoints; i++) {
        int j = (i + 1) % numPoints;
        DrawLine(points[i].x, points[i].y, points[j].x, points[j].y, BLUE);
      }
    }

    // Draw GUI controls
    GuiSliderBar((Rectangle){600, 40, 120, 20}, "Edge Blur", NULL, &edgeBlur,
                 0.1, 5.0);
    GuiSliderBar((Rectangle){600, 70, 120, 20}, "Smoothness", NULL, &smoothness,
                 0.0, 120.0);

    GuiCheckBox((Rectangle){600, 320, 20, 20}, "Draw Points", &drawPoints);
    GuiCheckBox((Rectangle){600, 350, 20, 20}, "Draw Outline", &drawOutline);

    DrawFPS(10, 10);
    EndDrawing();
  }

  return 0;
}
