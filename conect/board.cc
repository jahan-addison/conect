/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include <board.h>
#include <error.h>
#include <filesystem>
#include <iostream>
#include <nanogui/messagedialog.h>
#include <ranges>

namespace conect {

namespace fs = std::filesystem;

inline fs::path
get_resource(std::string_view file,
             std::string_view file_root = board::root_resource_directory)
{
    if (fs::exists(fs::path(file_root.data()) / file.data()))
        return fs::path(file_root.data()) / file.data();
    else
        return fs::path("");
}

resource::image
Board::load_resource(NVGcontext* ctx, resource::type type)
{
    switch (type) {
        case resource::type::board: {
            fs::path path = get_resource(BOARD_RESOURCE_FILE);
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case resource::type::red_coin: {
            auto path = get_resource(BOARD_RED_COIN_RESOURCE);
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case resource::type::blue_coin: {
            auto path = get_resource(BOARD_BLUE_COIN_RESOURCE);
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
    for (auto& row : layout)
        std::fill_n(row.begin(), row.size(), board::color::none);

    for (auto& row : i_layout) {
        for (auto& texture : row) {
            if (texture != 0) {
                nvgDeleteImage(ctx, texture);
                texture = 0;
            }
        }
    }
}

void
Board::print_board() const
{
    unsigned int count = 0;
    for (unsigned int i = 0; i < board::size::col; i++) {
        std::cout << " " << i;
    }
    std::cout << std::endl << "---------------" << std::endl;
    for (unsigned int r = 0; r < board::size::row; r++) {
        for (unsigned int c = 0; c < board::size::col; c++) {
            std::cout << "|";
            switch (layout[board::size::row - r - 1][c]) {
                case board::color::none:
                    std::cout << " ";
                    break;
                case board::color::red:
                    std::cout << "O";
                    count++;
                    break;
                case board::color::blue:
                    std::cout << "X";
                    count++;
                    break;
            }
            if (c + 1 == board::size::col) {
                std::cout << "|";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "---------------" << std::endl;
    std::cout << "Piece count: " << count << std::endl;
    std::cout << std::endl;
}

bool
Board::add_coin(NVGcontext* ctx, board::column column, board::color color)
{
    for (unsigned int r = 0; r < board::size::row; r++) {
        if (layout[r][column] == board::color::none) { // first available spot
            layout[r][column] = color;                 // set piece
            auto location = &i_layout[r];
            if (color == board::color::blue) {
                location->operator[](column) =
                  load_resource(ctx, resource::type::blue_coin);
            } else {
                location->operator[](column) =
                  load_resource(ctx, resource::type::red_coin);
            }
            if (location->operator[](column) == 0)
                error_exit_program_dialog(
                  this->parent(),
                  "Board::add_coin(): could not load coin texture");
            return true;
        }
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
        board = load_resource(ctx, resource::type::board);

    if (board == 0)
        error_exit_program_dialog(this->parent(),
                                  "Board::draw(): could not load board image");

    NVGpaint img_pattern = nvgImagePattern(
      ctx, m_pos.x(), 0, m_size.x(), m_size.y(), 0.f, board, 1.f);

    nvgFillPaint(ctx, img_pattern);

    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
    nvgFill(ctx);

    draw_coins(ctx);
}

} // namespace conect