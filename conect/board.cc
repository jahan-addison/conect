/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include <board.h>
#include <common.h>
#include <error.h>
#include <filesystem>
#include <iostream>
#include <nanogui/messagedialog.h>
#include <ranges>

namespace conect {

namespace fs = std::filesystem;

using nanogui::ref;
using nanogui::Vector2f;
using nanogui::Vector2i;

inline auto
get_resource(std::string_view file,
             std::string_view file_root = gui::root_resource_dir)
{
    if (std::filesystem::exists(fs::path(file_root.data()) / file.data()))
        return fs::path(file_root.data()) / file.data();
    else
        return fs::path("");
}

constexpr std::string_view
Board::column_to_string(gui::Column col)
{
    using Column = gui::Column;
    switch (col) {
        case Column::COL_E:
            return "none";
        case Column::COL_1:
            return "column 1";
        case Column::COL_2:
            return "column 2";
        case Column::COL_3:
            return "column 3";
        case Column::COL_4:
            return "column 4";
        case Column::COL_5:
            return "column 5";
        case Column::COL_6:
            return "column 6";
        case Column::COL_7:
            return "column 7";

        default:
            return "unknown column";
    }
}

constexpr int
Board::column_to_int(gui::Column col)
{
    using Column = gui::Column;
    switch (col) {
        case Column::COL_1:
            return 0;
        case Column::COL_2:
            return 1;
        case Column::COL_3:
            return 2;
        case Column::COL_4:
            return 3;
        case Column::COL_5:
            return 4;
        case Column::COL_6:
            return 5;
        case Column::COL_7:
            return 6;
        default:
            return 0;
    }
}

gui::Image
Board::load_resource(NVGcontext* ctx, gui::Resource_Type type)
{
    switch (type) {
        case gui::Resource_Type::BOARD: {
            fs::path path = get_resource(gui::board_resource);
            if (path.empty())
                error_exit_program_dialog(
                  this->parent(),
                  "Board::gui::load_resource(): could not load game board "
                  "image");

            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case gui::Resource_Type::RED_COIN: {
            auto path = get_resource(gui::red_coin_resource);
            if (path.empty())
                error_exit_program_dialog(this->parent(),
                                          "Board::gui::load_resource(): "
                                          "could not load red coin image");
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case gui::Resource_Type::BLUE_COIN: {
            auto path = get_resource(gui::blue_coin_resource);
            if (path.empty())
                error_exit_program_dialog(this->parent(),
                                          "Board::gui::load_resource(): "
                                          "could not load blue coin image");
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }

        default:
            error_exit_program_dialog(
              this->parent(), "Board::gui::load_resource(): invalid resource");
    }
    return 0;
}

inline std::pair<float, float>
Board::get_coin_drawing_pos(float x_pos, float y_pos) const noexcept
{
    float xx = m_pos.x() + 55.f + (106.2211f * x_pos);
    float yy = m_pos.y() + m_size.y() - 107.2222f - (75.6222f * y_pos);

    return { xx, yy };
}

void
Board::clear_board(NVGcontext* ctx)
{
    layout.fill({ gui::Color::NONE,
                  gui::Color::NONE,
                  gui::Color::NONE,
                  gui::Color::NONE,
                  gui::Color::NONE,
                  gui::Color::NONE });

    for (auto& column : i_layout) {
        for (auto& texture : column) {
            if (texture != 0) {
                nvgDeleteImage(ctx, texture);
                texture = 0;
            }
        }
    }
}

bool
Board::add_coin(NVGcontext* ctx, gui::Column col, gui::Color color)
{

    auto location = &i_layout[column_to_int(col)];
    auto test = std::ranges::find(location->begin(), location->end(), 0);
    if (test != location->end()) {
        this->layout[column_to_int(col)][test - location->begin()] = color;
        if (color == gui::Color::BLUE) {
            location->operator[](test - location->begin()) =
              load_resource(ctx, gui::Resource_Type::BLUE_COIN);
        } else {
            location->operator[](test - location->begin()) =
              load_resource(ctx, gui::Resource_Type::RED_COIN);
        }
        if (location->operator[](test - location->begin()) == 0)
            error_exit_program_dialog(
              this->parent(), "Board::add_coin(): could not load coin texture");

        return true;
    }
    return false;
}

void
Board::draw_coins(NVGcontext* ctx) const
{
    auto x_pos = 0, y_pos = 0;
    for (auto const& column : i_layout) {
        y_pos = 0;
        for (auto const& texture : column) {
            if (texture != 0) {
                auto col_position = get_coin_drawing_pos(
                  static_cast<float>(x_pos), static_cast<float>(y_pos));
                NVGpaint img_pattern = nvgImagePattern(ctx,
                                                       col_position.first,
                                                       col_position.second,
                                                       72.f,
                                                       72.f,
                                                       0.f,
                                                       texture,
                                                       1.f);

                nvgFillPaint(ctx, img_pattern);

                nvgRect(
                  ctx, col_position.first, col_position.second, 72.f, 72.f);
                nvgFill(ctx);
            }
            y_pos++;
        }
        x_pos++;
    }
}

void
Board::draw(NVGcontext* ctx)
{
    nanogui::Screen* scr = screen();
    if (scr == nullptr)
        error_exit_program_dialog(
          this->parent(), "Board::draw(): could not find parent screen!");
    if (board == -1)
        board = load_resource(ctx, gui::Resource_Type::BOARD);

    if (board == 0)
        error_exit_program_dialog(
          this->parent(), "Board::draw(): could not load game board image");

    NVGpaint img_pattern = nvgImagePattern(
      ctx, m_pos.x(), 0, m_size.x(), m_size.y(), 0.f, board, 1.f);

    nvgFillPaint(ctx, img_pattern);

    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
    nvgFill(ctx);

    draw_coins(ctx);
}

} // namespace conect