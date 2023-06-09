/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include "doctest/doctest.h"
#include <ai/stratzilla.h>
#include <gui.h>

using namespace conect;

// inline void
// clear_board_state_layout(gui::Layout* board)
// {
//     (*board).fill({ conect::gui::Color::NONE,
//                     conect::gui::Color::NONE,
//                     conect::gui::Color::NONE,
//                     conect::gui::Color::NONE,
//                     conect::gui::Color::NONE,
//                     conect::gui::Color::NONE });
// }

// inline void
// fill_board_state_layout(gui::Layout* board)
// {
//     (*board).fill({ conect::gui::Color::RED,
//                     conect::gui::Color::RED,
//                     conect::gui::Color::RED,
//                     conect::gui::Color::RED,
//                     conect::gui::Color::RED,
//                     conect::gui::Color::RED });
// }

TEST_CASE("Stratzilla algorithm horizontal winning move" * doctest::timeout(50))
{
    //     auto board = std::make_shared<gui::Layout>();
    //     auto engine = std::make_shared<Engine>();
    //     AI::ST::move moves{};
    //     auto st = AI::Stratzilla_Algorithm{ engine, board };

    //     // Horizontal tests

    //     // [0, 0, 0, 0, 0, 0, 0]
    //     // [0, 0, 0, 0, 0, 0, 0]
    //     // [0, 0, 0, 0, 0, 0, 0]
    //     // [0, 0, 0, 0, 0, 0, 0]
    //     // [0, 0, 0, 0, 0, 0, 0]
    //     // [2, 2, 2, 2, 0, 0, 0]

    //     clear_board_state_layout(st.board_.get());

    //     (*st.board_)[0][0] = gui::Color::RED;
    //     (*st.board_)[1][0] = gui::Color::BLUE;
    //     (*st.board_)[2][0] = gui::Color::RED;
    //     (*st.board_)[3][0] = gui::Color::BLUE;

    //     (*st.board_)[0][1] = gui::Color::BLUE;
    //     (*st.board_)[1][1] = gui::Color::BLUE;
    //     (*st.board_)[2][1] = gui::Color::NONE;
    //     (*st.board_)[3][1] = gui::Color::BLUE;

    //     // /clear_board_state_layout(st.board_.get());

    //     // Vertical tests

    //     //     // [0, 0, 0, 0, 0, 0, 0]
    //     //     // [0, 0, 0, 0, 0, 0, 0]
    //     //     // [2, 0, 0, 0, 0, 0, 0]
    //     //     // [2, 0, 0, 0, 0, 0, 0]
    //     //     // [2, 0, 0, 0, 0, 0, 0]
    //     //     // [2, 0, 0, 0, 0, 0, 0]

    //     // (*st.board_)[0][0] = gui::Color::RED;
    //     // (*st.board_)[1][0] = gui::Color::RED;
    //     // (*st.board_)[2][0] = gui::Color::RED;
    //     // (*st.board_)[3][0] = gui::Color::NONE;

    //     auto next_move = st.minimax_alpha_beta_pruning(
    //       board.get(), 10, 0 - INT_MAX, INT_MAX, gui::Color::BLUE);

    //     // auto test = st.get_next_move_is_winning(gui::Color::RED);
    //     // auto test = st.tabulate_score(gui::Color::RED);
    //     //  auto test = st.get_score_of_set(
    //     //    { gui::Color::RED, gui::Color::RED, gui::Color::RED,
    //     gui::Color::RED
    //     //    }, gui::Color::RED);
    //     // std::cout << "score: " << test;
    //     std::cout << "0: " << next_move[0] << " 1: " << next_move[1];
}
