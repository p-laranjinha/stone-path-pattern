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

int thickness = 3;
int tmp_thickness = thickness;
bool thickness_edit = false;

int max_x = 15;
int tmp_max_x = max_x;
bool max_x_edit = false;

int max_y = 10;
int tmp_max_y = max_y;
bool max_y_edit = false;

int pattern_choice = 0;
int pattern_scroll_index = 0;
bool show_original_pattern = false;
bool hide_pattern = false;
bool highlight_boundary = false;

float gui_start_x;
string pattern_choices;

// Specific pattern options.
int seed = 0;
int tmp_seed = seed;
bool seed_edit = false;
float diagonal_chance = 0.5;
int tmp_diagonal_chance = diagonal_chance * 100;
bool diagonal_chance_edit = false;
float right_diagonal_chance = 0.25;
int tmp_right_diagonal_chance = right_diagonal_chance * 100;
bool right_diagonal_chance_edit = false;

static void DrawDefaultGUI(void);

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  raylib::Window window(1000 + gui_panel_width, 1000, "stone-path-pattern");
  window.SetTargetFPS(1000);
  GuiLoadStyle("assets/style.rgs");
  GuiSetStyle(DEFAULT, TEXT_SIZE, text_size);

  vector<tuple<string, function<PatternWire()>, function<void()>>> patterns = {
      {"Random triangle",
       [&]() -> PatternWire {
         return randTrianglePattern(max_x, max_y, seed, diagonal_chance,
                                    right_diagonal_chance);
       },
       [&]() -> void {
         GuiLabel({gui_start_x, gui_padding + text_size * 23,
                   gui_input_width / 3, text_size},
                  "Seed:");
         if (GuiValueBox({gui_start_x, gui_padding + text_size * 24,
                          gui_input_width / 3, text_size},
                         "", &tmp_seed, 0, 99999, seed_edit)) {
           seed_edit = !seed_edit;
           if (!seed_edit) {
             seed = tmp_seed;
           }
         }
         GuiLabel({gui_start_x + gui_input_width / 3,
                   gui_padding + text_size * 23, gui_input_width / 3,
                   text_size},
                  "Diag. chance:");
         if (GuiValueBox(
                 {gui_start_x + gui_input_width / 3,
                  gui_padding + text_size * 24, gui_input_width / 3, text_size},
                 "", &tmp_diagonal_chance, 0, 100, diagonal_chance_edit)) {
           diagonal_chance_edit = !diagonal_chance_edit;
           if (!diagonal_chance_edit) {
             diagonal_chance = float(tmp_diagonal_chance) / 100;
           }
         }
         GuiLabel({gui_start_x + gui_input_width * 2 / 3,
                   gui_padding + text_size * 23, gui_input_width / 3,
                   text_size},
                  "Right d. chance:");
         if (GuiValueBox({gui_start_x + gui_input_width * 2 / 3,
                          gui_padding + text_size * 24, gui_input_width / 3,
                          text_size},
                         "", &tmp_right_diagonal_chance, 0, 100,
                         right_diagonal_chance_edit)) {
           right_diagonal_chance_edit = !right_diagonal_chance_edit;
           if (!right_diagonal_chance_edit) {
             right_diagonal_chance = float(tmp_right_diagonal_chance) / 100;
           }
         }
       }},
      {"Alternating triangle 2",
       [&]() -> PatternWire {
         return alternatingTriangleGridPattern2(max_x, max_y);
       },
       [&]() -> void {}},
      {"Alternating triangle",
       [&]() -> PatternWire {
         return alternatingTriangleGridPattern(max_x, max_y);
       },
       [&]() -> void {}},
      {"Triangle",
       [&]() -> PatternWire { return triangleGridPattern(max_x, max_y); },
       [&]() -> void {}},
      {"Grid", [&]() -> PatternWire { return gridPattern(max_x, max_y); },
       [&]() -> void {}},
  };
  for (int i = 0; i < patterns.size(); i++) {
    pattern_choices += get<0>(patterns[i]);
    if (i < patterns.size() - 1) {
      pattern_choices += ";";
    };
  }

  while (!window.ShouldClose()) { // Detect window close button or ESC key
    int pattern_width = window.GetRenderWidth() - padding * 2 - gui_panel_width;
    int pattern_height = window.GetRenderHeight() - padding * 2;
    gui_start_x = window.GetRenderWidth() - gui_panel_width + gui_padding;

    PatternWire wire;
    if (pattern_choice >= 0) {
      wire = get<1>(patterns[pattern_choice])();
    }
    PatternWire dual = dualMesh(wire);
    PatternWire dual_with_boundary = dualMeshWithBoundary(wire);

    while (window.Drawing()) {
      window.ClearBackground(
          raylib::Color(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      DrawDefaultGUI();
      get<2>(patterns[pattern_choice])();

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

void DrawDefaultGUI() {
  // if (GuiButton({window.GetRenderWidth() - gui_padding - 160,
  //                window.GetRenderHeight() - gui_padding - text_size, 160,
  //                text_size},
  //               "Take screenshot")) {
  //   TakeScreenshot("screenshot.png");
  // }
  GuiToggle({gui_start_x, gui_padding, gui_input_width, text_size},
            "Show original pattern", &show_original_pattern);

  GuiToggle(
      {gui_start_x, gui_padding + text_size * 3, gui_input_width, text_size},
      "Hide pattern", &hide_pattern);

  GuiToggle(
      {gui_start_x, gui_padding + text_size * 6, gui_input_width, text_size},
      "Highlight boundary", &highlight_boundary);

  GuiLabel({gui_start_x, gui_padding + text_size * 9, gui_input_width / 4,
            text_size},
           "Max X:");
  if (GuiValueBox({gui_start_x, gui_padding + text_size * 10,
                   gui_input_width / 4, text_size},
                  "", &tmp_max_x, 1, 1000, max_x_edit)) {
    max_x_edit = !max_x_edit;
    if (!max_x_edit) {
      max_x = tmp_max_x;
    }
  }

  GuiLabel({gui_start_x + gui_input_width / 4, gui_padding + text_size * 9,
            gui_input_width / 4, text_size},
           "Max Y:");
  if (GuiValueBox({gui_start_x + gui_input_width / 4,
                   gui_padding + text_size * 10, gui_input_width / 4,
                   text_size},
                  "", &tmp_max_y, 1, 1000, max_y_edit)) {
    max_y_edit = !max_y_edit;
    if (!max_y_edit) {
      max_y = tmp_max_y;
    }
  }

  GuiLabel({gui_start_x + gui_input_width / 2, gui_padding + text_size * 9,
            gui_input_width / 2, text_size},
           "Line thickness:");
  if (GuiValueBox({gui_start_x + gui_input_width / 2,
                   gui_padding + text_size * 10, gui_input_width / 2,
                   text_size},
                  "", &tmp_thickness, 1, 100, thickness_edit)) {
    thickness_edit = !thickness_edit;
    if (!thickness_edit) {
      thickness = tmp_thickness;
    }
  }

  GuiLabel(
      {gui_start_x, gui_padding + text_size * 13, gui_input_width, text_size},
      "Pattern:");
  GuiListView({gui_start_x, gui_padding + text_size * 14, gui_input_width,
               float(text_size * 7)},
              pattern_choices.c_str(), &pattern_scroll_index, &pattern_choice);
}
