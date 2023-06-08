/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <ai.h>
#include <common.h>
#include <engine.h>
#include <memory>
#include <nanogui/canvas.h>
#include <nanogui/opengl.h>
#include <nanogui/renderpass.h>
#include <nanogui/screen.h>
#include <ostream>
#include <string_view>
#include <utility>

#if defined(_WIN32)
#if defined(APIENTRY)
#undef APIENTRY
#endif
#include <windows.h>
#endif

namespace conect {

using nanogui::Canvas;

constexpr float Pi = 3.14159f;

class Engine;

class Board : public Canvas
{
  public:
    Board(Widget* parent, std::shared_ptr<Engine> engine)
      : Canvas(parent, 1)
      , engine(engine)
    {
    }
    void draw(NVGcontext* ctx) override;

  public:
    gui::Image load_resource(NVGcontext* ctx, gui::Resource_Type type);

  private:
    constexpr std::string_view column_to_string(gui::Column col);
    constexpr int column_to_int(gui::Column col);

  public:
    gui::Resource_Layout i_layout{};
    gui::Image board{ -1 };
    inline std::pair<float, float> get_coin_drawing_pos(
      float x_pos,
      float y_pos) const noexcept;
    void clear_board(NVGcontext* ctx);

    bool add_coin(NVGcontext* ctx, gui::Column col, gui::Color color);

    void draw_coins(NVGcontext* ctx) const;

  public:
    gui::Layout layout{};
    std::shared_ptr<Engine> engine;
};

} // namespace conect