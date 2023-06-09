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
#include <string_view>
#include <utility>

namespace conect {

class Engine;

class Board : public nanogui::Canvas
{
  public:
    Board(Widget* parent, std::shared_ptr<Engine> engine)
      : nanogui::Canvas(parent, 1)
      , engine(engine)
    {
    }
    void draw(NVGcontext* ctx) override;

  public:
    resource::image load_resource(NVGcontext* ctx, resource::type type);

  public:
    resource::layout i_layout{};
    resource::image board{ -1 };
    inline std::pair<float, float> get_coin_drawing_pos(
      float x_pos,
      float y_pos) const noexcept;
    void clear_board(NVGcontext* ctx);

    void print_board() const;

    bool add_coin(NVGcontext* ctx, board::column column, board::color color);

    void draw_coins(NVGcontext* ctx) const;

  public:
    board::layout layout{};
    std::shared_ptr<Engine> engine;
};

} // namespace conect