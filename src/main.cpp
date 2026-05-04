#include "patterns/alt_triangle_grid.hpp"
#include "patterns/alt_triangle_grid2.hpp"
#include "patterns/grid.hpp"
#include "patterns/rand_triangle.hpp"
#include "patterns/triangle_grid.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <functional>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib-cpp.hpp" // IWYU pragma: export

using namespace std;
using namespace Types;

int padding = 5;
int gui_panel_width = 400;
float gui_padding = 20;
float gui_input_width = gui_panel_width - gui_padding * 2;
float text_size = 18;

IntInputValue thickness = {3, 3, false};
IntInputValue max_x = {15, 15, false};
IntInputValue max_y = {10, 10, false};

int pattern_choice = 0;
int pattern_scroll_index = 0;
bool show_original_pattern = false;
bool hide_pattern = false;
bool highlight_boundary = false;

float gui_start_x;
string pattern_choices;
vector<tuple<string, function<PatternWire()>, function<void()>>> patterns;
int pattern_width;
int pattern_height;

// Specific pattern options.
IntInputValue seed = {1, 1, false};
IntInputValue diagonal_chance = {50, 50, false};
IntInputValue right_diagonal_chance = {25, 25, false};

static void DrawPattern(void);
static void DrawDefaultGUI(void);

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  raylib::Window window(1000 + gui_panel_width, 1000, "stone-path-pattern");
  window.SetTargetFPS(1000);
  GuiLoadStyle("assets/style.rgs");
  GuiSetStyle(DEFAULT, TEXT_SIZE, text_size);

  patterns = {
      {"Random triangle",
       [&]() -> PatternWire {
         return randTrianglePattern(get<0>(max_x), get<0>(max_y),
                                    float(get<0>(diagonal_chance)) / 100,
                                    float(get<0>(right_diagonal_chance)) / 100);
       },
       [&]() -> void {
         GuiLabel({gui_start_x, gui_padding + text_size * 23,
                   gui_input_width / 3, text_size},
                  "Seed:");
         DrawIntInput(seed,
                      {gui_start_x, gui_padding + text_size * 24,
                       gui_input_width / 3, text_size},
                      1, 99999);
         GuiLabel({gui_start_x + gui_input_width / 3,
                   gui_padding + text_size * 23, gui_input_width / 3,
                   text_size},
                  "Diag. chance:");
         DrawIntInput(diagonal_chance,
                      {gui_start_x + gui_input_width / 3,
                       gui_padding + text_size * 24, gui_input_width / 3,
                       text_size},
                      0, 100);
         GuiLabel({gui_start_x + gui_input_width * 2 / 3,
                   gui_padding + text_size * 23, gui_input_width / 3,
                   text_size},
                  "Right d. chance:");
         DrawIntInput(right_diagonal_chance,
                      {gui_start_x + gui_input_width * 2 / 3,
                       gui_padding + text_size * 24, gui_input_width / 3,
                       text_size},
                      0, 100);
       }},
      {"Alternating triangle 2",
       [&]() -> PatternWire {
         return alternatingTriangleGridPattern2(get<0>(max_x), get<0>(max_y));
       },
       [&]() -> void {}},
      {"Alternating triangle",
       [&]() -> PatternWire {
         return alternatingTriangleGridPattern(get<0>(max_x), get<0>(max_y));
       },
       [&]() -> void {}},
      {"Triangle",
       [&]() -> PatternWire {
         return triangleGridPattern(get<0>(max_x), get<0>(max_y));
       },
       [&]() -> void {}},
      {"Grid",
       [&]() -> PatternWire {
         return gridPattern(get<0>(max_x), get<0>(max_y));
       },
       [&]() -> void {}},
  };
  for (int i = 0; i < patterns.size(); i++) {
    pattern_choices += get<0>(patterns[i]);
    if (i < patterns.size() - 1) {
      pattern_choices += ";";
    };
  }

  while (!window.ShouldClose()) { // Detect window close button or ESC key
    srand(get<0>(seed));

    pattern_width = window.GetRenderWidth() - padding * 2 - gui_panel_width;
    pattern_height = window.GetRenderHeight() - padding * 2;
    gui_start_x = window.GetRenderWidth() - gui_panel_width + gui_padding;

    while (window.Drawing()) {
      window.ClearBackground(
          raylib::Color(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      DrawDefaultGUI();

      if (pattern_choice < 0) {
        continue;
      }

      DrawPattern();

      window.DrawFPS(gui_start_x,
                     window.GetRenderHeight() - gui_padding - text_size);
    }
  }

  return 0;
}

void DrawPattern() {
  get<2>(patterns[pattern_choice])();
  PatternWire wire = get<1>(patterns[pattern_choice])();
  randomizeCenterPositions(wire, 0.1);

  if (show_original_pattern && hide_pattern) {
    PatternPreFill polylines = wireToPolylines(wire);
    PatternFill fill = polylinesTriangulation(polylines);
    DrawFill(fill, get<0>(max_x), get<0>(max_y), pattern_width, pattern_height,
             padding, padding, raylib::Color::DarkBlue());
    DrawEdges(wire, get<0>(max_x), get<0>(max_y), get<0>(thickness),
              pattern_width, pattern_height, padding, padding,
              raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
  }
  if (!hide_pattern) {
    PatternWire dual_with_boundary = dualMeshWithBoundary(wire);
    if (!highlight_boundary) {
      PatternPreFill polylines = wireToPolylines(dual_with_boundary);
      PatternFill fill = polylinesTriangulation(polylines);
      DrawFill(fill, get<0>(max_x), get<0>(max_y), pattern_width,
               pattern_height, padding, padding, raylib::Color::DarkBlue());
      if (show_original_pattern) {
        DrawEdges(wire, get<0>(max_x), get<0>(max_y), get<0>(thickness),
                  pattern_width, pattern_height, padding, padding,
                  raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED)));
      }
      DrawEdges(dual_with_boundary, get<0>(max_x), get<0>(max_y),
                get<0>(thickness), pattern_width, pattern_height, padding,
                padding,
                raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
    } else {
      PatternWire dual = dualMesh(wire);
      PatternPreFill polylines = wireToPolylines(dual);
      PatternFill fill = polylinesTriangulation(polylines);
      DrawFill(fill, get<0>(max_x), get<0>(max_y), pattern_width,
               pattern_height, padding, padding, raylib::Color::DarkBlue());
      if (show_original_pattern) {
        DrawEdges(wire, get<0>(max_x), get<0>(max_y), get<0>(thickness),
                  pattern_width, pattern_height, padding, padding,
                  raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED)));
      }
      DrawEdges(dual_with_boundary, get<0>(max_x), get<0>(max_y),
                get<0>(thickness), pattern_width, pattern_height, padding,
                padding,
                raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
      DrawEdges(dual, get<0>(max_x), get<0>(max_y), get<0>(thickness),
                pattern_width, pattern_height, padding, padding,
                raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
    }
  }
}

void DrawDefaultGUI() {
  // if (GuiButton({window.GetRenderWidth() - gui_padding - 160,
  //                window.GetRenderHeight() - gui_padding - text_size, 160,
  //                text_size},
  //               "Take screenshot")) {
  //   TakeScreenshot("screenshot.png");
  // }
  GuiCheckBox({gui_start_x, gui_padding, text_size, text_size},
              "Show original pattern", &show_original_pattern);

  GuiCheckBox({gui_start_x, gui_padding + text_size * 3, text_size, text_size},
              "Hide pattern", &hide_pattern);

  GuiCheckBox({gui_start_x, gui_padding + text_size * 6, text_size, text_size},
              "Highlight boundary", &highlight_boundary);

  GuiLabel({gui_start_x, gui_padding + text_size * 9, gui_input_width / 4,
            text_size},
           "Max X:");
  DrawIntInput(max_x,
               {gui_start_x, gui_padding + text_size * 10, gui_input_width / 4,
                text_size},
               0, 1000);
  GuiLabel({gui_start_x + gui_input_width / 4, gui_padding + text_size * 9,
            gui_input_width / 4, text_size},
           "Max Y:");
  DrawIntInput(max_y,
               {gui_start_x + gui_input_width / 4, gui_padding + text_size * 10,
                gui_input_width / 4, text_size},
               0, 1000);
  GuiLabel({gui_start_x + gui_input_width / 2, gui_padding + text_size * 9,
            gui_input_width / 2, text_size},
           "Line thickness:");
  DrawIntInput(thickness,
               {gui_start_x + gui_input_width / 2, gui_padding + text_size * 10,
                gui_input_width / 2, text_size},
               0, 100);

  GuiLabel(
      {gui_start_x, gui_padding + text_size * 13, gui_input_width, text_size},
      "Pattern:");
  GuiListView({gui_start_x, gui_padding + text_size * 14, gui_input_width,
               float(text_size * 7)},
              pattern_choices.c_str(), &pattern_scroll_index, &pattern_choice);
}
