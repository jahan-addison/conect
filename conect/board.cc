/*
    conect is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    conect is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with conect.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <board.h>
#include <error.h>
#include <filesystem>
#include <iostream>
#include <nanogui/messagedialog.h>
#include <ranges>
#include <resource.h>

namespace conect {

namespace fs = std::filesystem;

using nanogui::ref;
using nanogui::Vector2f;
using nanogui::Vector2i;

inline auto
get_resource(std::string_view file,
             std::string_view file_root = resource::root_resource_dir)
{
    if (std::filesystem::exists(fs::path(file_root.data()) / file.data()))
        return fs::path(file_root.data()) / file.data();
    else
        return fs::path("");
}

constexpr std::string_view
Board::column_to_string(resource::Column col)
{
    using Column = resource::Column;
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
Board::column_to_int(resource::Column col)
{
    using Column = resource::Column;
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

resource::Image
Board::load_resource(NVGcontext* ctx, resource::Resource_Type type)
{
    switch (type) {
        case resource::Resource_Type::BOARD: {
            fs::path path = get_resource(resource::board_resource);
            if (path.empty())
                error_exit_program_dialog(
                  this->parent(),
                  "Board::resource::load_resource(): could not load game board "
                  "image");

            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case resource::Resource_Type::RED_COIN: {
            auto path = get_resource(resource::red_coin_resource);
            if (path.empty())
                error_exit_program_dialog(this->parent(),
                                          "Board::resource::load_resource(): "
                                          "could not load red coin image");
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case resource::Resource_Type::BLUE_COIN: {
            auto path = get_resource(resource::blue_coin_resource);
            if (path.empty())
                error_exit_program_dialog(this->parent(),
                                          "Board::resource::load_resource(): "
                                          "could not load blue coin image");
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }

        default:
            error_exit_program_dialog(
              this->parent(),
              "Board::resource::load_resource(): invalid resource");
    }
    return 0;
}

// Board::Color
// Board::State::get_in_a_row_same_color_of_four() const
// {
//     auto [row_c, col_c] =
//       std::make_tuple(Engine::Color::NONE, Engine::Color::NONE);
//     auto adjacent_r = 0, adjacent_c = 0;
//     for (auto const& row : layout) {
//         auto cols = std::begin(row);
//         if ((row_c == Engine::Color::BLUE or row_c == Engine::Color::RED) and
//             row_c == *cols)
//             adjacent_r++;
//         else if (row_c != *cols)
//             adjacent_r = 0;
//         row_c = *cols;
//         if (adjacent_r > 2) {
//             return *cols;
//         }
//         col_c = Engine::Color::NONE;
//         adjacent_c = 0;
//         for (auto end = std::end(row); cols != end; cols++) {
//             if ((col_c == Engine::Color::BLUE or
//                  col_c == Engine::Color::RED) and
//                 col_c == *cols)
//                 adjacent_c++;
//             if (adjacent_c > 2) {
//                 return *cols;
//             }
//             col_c = *cols;
//         }
//     }
//     return Engine::Color::NONE;
// }

// Engine::Color
// Board::State::get_diagonal_same_color_of_four(bool start_left = true) const
// {
//     const int size = layout.size();
//     const int row_size = layout.front().size();

//     auto color = Engine::Color::NONE;
//     auto last = Engine::Color::NONE;

//     int adjacent_l = 0;
//     int start_col = 0;
//     int start_row = 0;
//     int last_row = size - 1;

//     int row;

//     // forward diagonal search
//     if (start_left) {
//         while (start_row < size or start_col < row_size) {
//             if (start_row < size)
//                 row = start_row++;
//             else {
//                 row = last_row;
//                 start_col++;
//             }
//             last = Engine::Color::NONE;
//             adjacent_l = 0;
//             for (int col = start_col; col < row_size and row >= 0; col++) {
//                 if ((layout[row][col] == Engine::Color::BLUE or
//                      layout[row][col] == Engine::Color::RED) and
//                     last == layout[row][col])
//                     adjacent_l++;
//                 else
//                     adjacent_l = 0;
//                 if (adjacent_l > 2)
//                     return last;
//                 if (layout[row][col] == Engine::Color::BLUE or
//                     layout[row][col] == Engine::Color::RED)
//                     last = layout[row][col];
//                 else
//                     last = Engine::Color::NONE;
//                 row--;
//             }
//         }
//     }
//     // backward diagonal search
//     else {
//         start_col = layout.back().size();
//         while (start_row < size or start_col >= 0) {
//             if (start_row < size)
//                 row = start_row++;
//             else {
//                 row = last_row;
//                 start_col--;
//             }
//             last = Engine::Color::NONE;
//             adjacent_l = 0;
//             for (int col = start_col; col >= 0 and row >= 0; col--) {
//                 if ((layout[row][col] == Engine::Color::BLUE or
//                      layout[row][col] == Engine::Color::RED) and
//                     last == layout[row][col])
//                     adjacent_l++;
//                 else
//                     adjacent_l = 0;
//                 if (adjacent_l > 2)
//                     return last;
//                 if (layout[row][col] == Engine::Color::BLUE or
//                     layout[row][col] == Engine::Color::RED)
//                     last = layout[row][col];
//                 else
//                     last = Engine::Color::NONE;
//                 row--;
//             }
//         }
//     }

//     return color;
// }

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
    layout.fill({ resource::Color::NONE,
                  resource::Color::NONE,
                  resource::Color::NONE,
                  resource::Color::NONE,
                  resource::Color::NONE,
                  resource::Color::NONE });

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
Board::add_coin(NVGcontext* ctx, resource::Column col, resource::Color color)
{

    auto location = &i_layout[column_to_int(col)];
    auto test = std::ranges::find(location->begin(), location->end(), 0);
    if (test != location->end()) {
        this->layout[column_to_int(col)][test - location->begin()] = color;
        if (color == resource::Color::BLUE) {
            location->operator[](test - location->begin()) =
              load_resource(ctx, resource::Resource_Type::BLUE_COIN);
        } else {
            location->operator[](test - location->begin()) =
              load_resource(ctx, resource::Resource_Type::RED_COIN);
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
        board = load_resource(ctx, resource::Resource_Type::BOARD);

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