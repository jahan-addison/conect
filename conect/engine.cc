/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include <engine.h>

namespace conect {

void
Engine::set_next_player()
{
    if (*player == player_1)
        player = &player_2;
    else
        player = &player_1;
}

Engine::Player*
Engine::get_player(Players p)
{
    switch (p) {
        case Engine::Players::First:
            return &player_1;
        case Engine::Players::Second:
            return &player_2;
        default:
            return &player_1;
    }
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

std::optional<Engine::Player>
Engine::is_won() const
{
    // auto color = this->get_diagonal_same_color_of_four();
    // if (color == Engine::Color::NONE)
    //     color = this->get_diagonal_same_color_of_four(false);
    // if (color == Engine::Color::NONE)
    //     color = this->get_in_a_row_same_color_of_four();
    return std::nullopt;
}

bool
Engine::is_full(gui::Layout layout) const
{
    bool full = true;
    for (auto const& k : layout) {
        for (auto const& j : k)
            if (j != gui::Color::RED and j != gui::Color::BLUE)
                full = false;
    }
    return full;
}

} // namespace conect