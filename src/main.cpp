#include "patterns/alt_triangle_grid.hpp"
#include "patterns/alt_triangle_grid2.hpp"
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
  int gui_panel_width = 400;
  float gui_padding = 20;
  float gui_input_width = gui_panel_width - gui_padding * 2;
  float text_size = 20;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  raylib::Window window(1000 + gui_panel_width, 1000, "stone-path-pattern");
  window.SetTargetFPS(1000);
  GuiLoadStyle("assets/style.rgs");
  GuiSetStyle(DEFAULT, TEXT_SIZE, text_size);

  int thickness = 3;
  int tmp_thickness = thickness;
  bool thickness_edit = false;

  int max_x = 15;
  int tmp_max_x = max_x;
  bool max_x_edit = false;

  int max_y = 10;
  int tmp_max_y = max_y;
  bool max_y_edit = false;

  int pattern_choice = 3;
  bool show_original_pattern = false;
  bool hide_pattern = false;
  bool highlight_boundary = false;

  // Main game loop
  while (!window.ShouldClose()) { // Detect window close button or ESC key

    if (GuiButton({window.GetRenderWidth() - gui_padding - 160,
                   window.GetRenderHeight() - gui_padding - text_size, 160,
                   text_size},
                  "Take screenshot")) {
      TakeScreenshot("screenshot.png");
    }
    int pattern_width = window.GetRenderWidth() - padding * 2 - gui_panel_width;
    int pattern_height = window.GetRenderHeight() - padding * 2;
    float gui_start_x = window.GetRenderWidth() - gui_panel_width + gui_padding;

    PatternWire wire;
    switch (pattern_choice) {
    case 0:
      wire = gridPattern(max_x, max_y);
      break;
    case 1:
      wire = triangleGridPattern(max_x, max_y);
      break;
    case 2:
      wire = alternatingTriangleGridPattern(max_x, max_y);
      break;
    case 3:
      wire = alternatingTriangleGridPattern2(max_x, max_y);
      break;
    }
    PatternWire dual = dualMesh(wire);
    PatternWire dual_with_boundary = dualMeshWithBoundary(wire);

    while (window.Drawing()) {
      window.ClearBackground(
          raylib::Color(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      GuiToggle({gui_start_x, gui_padding, gui_input_width, text_size},
                "Show original pattern", &show_original_pattern);

      GuiToggle({gui_start_x, gui_padding + text_size * 3, gui_input_width,
                 text_size},
                "Hide pattern", &hide_pattern);

      GuiToggle({gui_start_x, gui_padding + text_size * 6, gui_input_width,
                 text_size},
                "Highlight boundary", &highlight_boundary);

      GuiLabel({gui_start_x, gui_padding + text_size * 9, gui_input_width,
                text_size},
               "Max X:");
      if (GuiValueBox({gui_start_x, gui_padding + text_size * 10,
                       gui_input_width, text_size},
                      "", &tmp_max_x, 1, 1000, max_x_edit)) {
        max_x_edit = !max_x_edit;
        if (!max_x_edit) {
          max_x = tmp_max_x;
        }
      }

      GuiLabel({gui_start_x, gui_padding + text_size * 13, gui_input_width,
                text_size},
               "Max Y:");
      if (GuiValueBox({gui_start_x, gui_padding + text_size * 14,
                       gui_input_width, text_size},
                      "", &tmp_max_y, 1, 1000, max_y_edit)) {
        max_y_edit = !max_y_edit;
        if (!max_y_edit) {
          max_y = tmp_max_y;
        }
      }

      GuiLabel({gui_start_x, gui_padding + text_size * 17, gui_input_width,
                text_size},
               "Line thickness:");
      if (GuiValueBox({gui_start_x, gui_padding + text_size * 18,
                       gui_input_width, text_size},
                      "", &tmp_thickness, 1, 100, thickness_edit)) {
        thickness_edit = !thickness_edit;
        if (!thickness_edit) {
          thickness = tmp_thickness;
        }
      }

      GuiLabel({gui_start_x, gui_padding + text_size * 21, gui_input_width,
                text_size},
               "Pattern:");
      GuiDropdownBox(
          {gui_start_x, gui_padding + text_size * 22, gui_input_width,
           text_size},
          "Grid;Triangle;Alternating Triangle;Alternating Triangle 2",
          &pattern_choice, true);

      if (show_original_pattern) {
        DrawEdges(wire, max_x, max_y, thickness, pattern_width, pattern_height,
                  padding, padding,
                  raylib::Color(GuiGetStyle(
                      DEFAULT, (hide_pattern) ? BORDER_COLOR_FOCUSED
                                              : BORDER_COLOR_PRESSED)));
      }
      if (!hide_pattern) {
        DrawEdges(dual_with_boundary, max_x, max_y, thickness, pattern_width,
                  pattern_height, padding, padding,
                  raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
        if (highlight_boundary) {
          DrawEdges(dual, max_x, max_y, thickness, pattern_width,
                    pattern_height, padding, padding,
                    raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
        }
      }

      window.DrawFPS(gui_start_x,
                     window.GetRenderHeight() - gui_padding - text_size);
    }
  }

  return 0;
}
