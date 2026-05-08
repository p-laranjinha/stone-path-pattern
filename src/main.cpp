#include "patterns/alt_triangle_grid.hpp"
#include "patterns/alt_triangle_grid2.hpp"
#include "patterns/grid.hpp"
#include "patterns/rand_triangle.hpp"
#include "patterns/triangle_grid.hpp"
#include "types.hpp"
#include "utils.hpp"
#include <format>
#include <functional>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib-cpp.hpp" // IWYU pragma: export

using namespace std;
using namespace Types;

int padding = 5;
int gui_panel_width = 400;
float gui_padding = 20;
float gui_spacing = 4;
float gui_input_width = gui_panel_width - gui_padding * 2;
float text_size = 18;

struct State {
  IntInputValue thickness = {1, 1, false};
  IntInputValue max_x = {15, 15, false};
  IntInputValue max_y = {15, 15, false};
  IntInputValue seed = {1, 1, false};
  IntInputValue center_max_offset = {20, 20, false};
  int pattern_choice = 0;
  bool show_original_pattern = false;
  bool hide_pattern = false;
  bool highlight_boundary = false;
  Color base_color = raylib::Color(169, 169, 169);
  Color hover_color = raylib::Color(220, 120, 45);

  // Specific pattern options.
  IntInputValue diagonal_chance = {50, 50, false};
  IntInputValue right_diagonal_chance = {25, 25, false};

  bool operator==(const State &other) const {
    return thickness.value == other.thickness.value &&
           max_x.value == other.max_x.value &&
           max_y.value == other.max_y.value && seed.value == other.seed.value &&
           center_max_offset.value == other.center_max_offset.value &&
           pattern_choice == other.pattern_choice &&
           show_original_pattern == other.show_original_pattern &&
           hide_pattern == other.hide_pattern &&
           highlight_boundary == other.highlight_boundary &&
           base_color.r == other.base_color.r &&
           base_color.g == other.base_color.g &&
           base_color.b == other.base_color.b &&
           base_color.a == other.base_color.a &&
           hover_color.r == other.hover_color.r &&
           hover_color.g == other.hover_color.g &&
           hover_color.b == other.hover_color.b &&
           hover_color.a == other.hover_color.a &&
           diagonal_chance.value == other.diagonal_chance.value &&
           right_diagonal_chance.value == other.right_diagonal_chance.value;
  }
} state;

struct Pattern {
  string name;
  function<PatternWire()> generator_func;
  function<void()> gui_func;
};

float gui_start_x;
string pattern_choices;
vector<Pattern> patterns;
int pattern_width;
int pattern_height;
PatternWire wire;
PatternWire dual;
PatternWire dual_with_boundary;
PatternFill pattern_fill;
int pattern_scroll_index = 0;

static void GeneratePattern();
static void DrawPattern();
static void DrawDefaultGUI();

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  raylib::Window window(1000 + gui_panel_width, 1000, "stone-path-pattern");
  GuiLoadStyle("assets/style.rgs");
  GuiSetStyle(DEFAULT, TEXT_SIZE, text_size);
  window.SetTargetFPS(60);

  patterns = {
      {"Random triangle",
       [&]() -> PatternWire {
         return randTrianglePattern(state.max_x.value, state.max_y.value,
                                    float(state.diagonal_chance.value) / 100,
                                    float(state.right_diagonal_chance.value) /
                                        100);
       },
       [&]() -> void {
         GuiLabel({gui_start_x, gui_padding + text_size * 30,
                   gui_input_width / 2 - gui_spacing / 2, text_size},
                  "Diag. chance:");
         DrawIntInput(state.diagonal_chance,
                      {gui_start_x, gui_padding + text_size * 31,
                       gui_input_width / 2 - gui_spacing / 2, text_size},
                      0, 100);
         GuiLabel({gui_start_x + gui_input_width / 2 + gui_spacing / 2,
                   gui_padding + text_size * 30,
                   gui_input_width / 2 - gui_spacing / 2, text_size},
                  "Right d. chance:");
         DrawIntInput(state.right_diagonal_chance,
                      {gui_start_x + gui_input_width / 2 + gui_spacing / 2,
                       gui_padding + text_size * 31,
                       gui_input_width / 2 - gui_spacing / 2, text_size},
                      0, 100);
       }},
      {"Alternating triangle 2",
       [&]() -> PatternWire {
         return alternatingTriangleGridPattern2(state.max_x.value,
                                                state.max_y.value);
       },
       [&]() -> void {}},
      {"Alternating triangle",
       [&]() -> PatternWire {
         return alternatingTriangleGridPattern(state.max_x.value,
                                               state.max_y.value);
       },
       [&]() -> void {}},
      {"Triangle",
       [&]() -> PatternWire {
         return triangleGridPattern(state.max_x.value, state.max_y.value);
       },
       [&]() -> void {}},
      {"Grid",
       [&]() -> PatternWire {
         return gridPattern(state.max_x.value, state.max_y.value);
       },
       [&]() -> void {}},
  };
  for (int i = 0; i < patterns.size(); i++) {
    pattern_choices += patterns[i].name;
    if (i < patterns.size() - 1) {
      pattern_choices += ";";
    };
  }

  State old_state;
  bool first_render = true;
  int seed = state.seed.value;
  while (!window.ShouldClose()) { // Detect window close button or ESC key
    pattern_width = window.GetRenderWidth() - padding * 2 - gui_panel_width;
    pattern_height = window.GetRenderHeight() - padding * 2;
    gui_start_x = window.GetRenderWidth() - gui_panel_width + gui_padding;

    while (window.Drawing()) {
      window.ClearBackground(
          raylib::Color(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      DrawDefaultGUI();
      window.DrawFPS(gui_start_x,
                     window.GetRenderHeight() - gui_padding - text_size);

      if (state.pattern_choice < 0) {
        continue;
      }

      patterns[state.pattern_choice].gui_func();
      srand(seed);
      DrawPattern();

      if (!(state == old_state) || first_render) {
        first_render = false;
        seed = state.seed.value;
        srand(seed);
        GeneratePattern();
      }

      old_state = state;
    }
  }

  return 0;
}

void GeneratePattern() {
  wire = patterns[state.pattern_choice].generator_func();
  randomizeCenterPositions(wire, float(state.center_max_offset.value) / 100);
  if (state.show_original_pattern && state.hide_pattern) {
    PatternPolylines polylines = wireToPolylines(wire);
    pattern_fill = polylinesTriangulation(polylines);
  }
  if (!state.hide_pattern) {
    dual_with_boundary =
        dualMeshWithBoundary(wire, float(state.center_max_offset.value) / 100,
                             state.max_x.value, state.max_y.value);
    if (!state.highlight_boundary) {
      PatternPolylines polylines = wireToPolylines(dual_with_boundary);
      pattern_fill = polylinesTriangulation(polylines);
    } else {
      dual = dualMesh(wire);
      PatternPolylines polylines = wireToPolylines(dual);
      pattern_fill = polylinesTriangulation(polylines);
    }
  }
}

void DrawPattern() {
  if (state.show_original_pattern && state.hide_pattern) {
    DrawFill(pattern_fill, state.max_x.value, state.max_y.value, pattern_width,
             pattern_height, padding, padding, state.base_color,
             state.hover_color);
    DrawEdges(wire, state.max_x.value, state.max_y.value, state.thickness.value,
              pattern_width, pattern_height, padding, padding,
              raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED)));
  }
  if (!state.hide_pattern) {
    if (!state.highlight_boundary) {
      DrawFill(pattern_fill, state.max_x.value, state.max_y.value,
               pattern_width, pattern_height, padding, padding,
               state.base_color, state.hover_color);
      if (state.show_original_pattern) {
        DrawEdges(wire, state.max_x.value, state.max_y.value,
                  state.thickness.value, pattern_width, pattern_height, padding,
                  padding,
                  raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
      }
      DrawEdges(dual_with_boundary, state.max_x.value, state.max_y.value,
                state.thickness.value, pattern_width, pattern_height, padding,
                padding,
                raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED)));
    } else {
      DrawFill(pattern_fill, state.max_x.value, state.max_y.value,
               pattern_width, pattern_height, padding, padding,
               state.base_color, state.hover_color);
      if (state.show_original_pattern) {
        DrawEdges(wire, state.max_x.value, state.max_y.value,
                  state.thickness.value, pattern_width, pattern_height, padding,
                  padding,
                  raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_FOCUSED)));
      }
      DrawEdges(dual_with_boundary, state.max_x.value, state.max_y.value,
                state.thickness.value, pattern_width, pattern_height, padding,
                padding,
                raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));
      DrawEdges(dual, state.max_x.value, state.max_y.value,
                state.thickness.value, pattern_width, pattern_height, padding,
                padding,
                raylib::Color(GuiGetStyle(DEFAULT, BORDER_COLOR_PRESSED)));
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
              "Show original pattern", &state.show_original_pattern);

  GuiCheckBox({gui_start_x, gui_padding + text_size * 2, text_size, text_size},
              "Hide pattern", &state.hide_pattern);

  GuiCheckBox({gui_start_x, gui_padding + text_size * 4, text_size, text_size},
              "Highlight boundary", &state.highlight_boundary);

  GuiLabel({gui_start_x, gui_padding + text_size * 6,
            gui_input_width / 4 - gui_spacing / 2, text_size},
           "Max X:");
  DrawIntInput(state.max_x,
               {gui_start_x, gui_padding + text_size * 7,
                gui_input_width / 4 - gui_spacing / 2, text_size},
               0, 1000);
  GuiLabel({gui_start_x + gui_input_width / 4 + gui_spacing / 2,
            gui_padding + text_size * 6, gui_input_width / 4 - gui_spacing,
            text_size},
           "Max Y:");
  DrawIntInput(state.max_y,
               {gui_start_x + gui_input_width / 4 + gui_spacing / 2,
                gui_padding + text_size * 7, gui_input_width / 4 - gui_spacing,
                text_size},
               0, 1000);
  GuiLabel({gui_start_x + gui_input_width / 2 + gui_spacing / 2,
            gui_padding + text_size * 6, gui_input_width / 2 - gui_spacing / 2,
            text_size},
           "Line thickness:");
  DrawIntInput(state.thickness,
               {gui_start_x + gui_input_width / 2 + gui_spacing / 2,
                gui_padding + text_size * 7,
                gui_input_width / 2 - gui_spacing / 2, text_size},
               0, 100);

  GuiLabel({gui_start_x, gui_padding + text_size * 9,
            gui_input_width / 2 - gui_spacing / 2, text_size},
           "Seed:");
  DrawIntInput(state.seed,
               {gui_start_x, gui_padding + text_size * 10,
                gui_input_width / 2 - gui_spacing / 2, text_size},
               1, 99999);
  GuiLabel({gui_start_x + gui_input_width / 2 + gui_spacing / 2,
            gui_padding + text_size * 9, gui_input_width / 2 - gui_spacing / 2,
            text_size},
           "Max point offset * 100:");
  DrawIntInput(state.center_max_offset,
               {gui_start_x + gui_input_width / 2 + gui_spacing / 2,
                gui_padding + text_size * 10,
                gui_input_width / 2 - gui_spacing / 2, text_size},
               0, 100);

  GuiLabel(
      {gui_start_x, gui_padding + text_size * 12, gui_input_width, text_size},
      "Base color:");
  GuiColorPicker(
      {gui_start_x, gui_padding + text_size * 13, text_size * 7, text_size * 7},
      "", &state.base_color);
  GuiLabel(
      {gui_start_x, gui_padding + text_size * 19, gui_input_width, text_size},
      format("{},{},{}", state.base_color.r, state.base_color.g,
             state.base_color.b)
          .c_str());

  GuiLabel({gui_start_x + gui_input_width / 2, gui_padding + text_size * 12,
            gui_input_width, text_size},
           "Hover color:");
  GuiColorPicker({gui_start_x + gui_input_width / 2,
                  gui_padding + text_size * 13, text_size * 7, text_size * 7},
                 "", &state.hover_color);
  GuiLabel({gui_start_x + gui_input_width / 2, gui_padding + text_size * 19,
            gui_input_width, text_size},
           format("{},{},{}", state.hover_color.r, state.hover_color.g,
                  state.hover_color.b)
               .c_str());

  GuiLabel(
      {gui_start_x, gui_padding + text_size * 21, gui_input_width, text_size},
      "Pattern:");
  GuiListView({gui_start_x, gui_padding + text_size * 22, gui_input_width,
               text_size * 7},
              pattern_choices.c_str(), &pattern_scroll_index,
              &state.pattern_choice);
}
