#include "doctest/doctest.h"
#include <board.h>
#include <iostream>

using namespace linea;

inline void clear_board_state_layout(Board::State &state)
{
    state.layout.fill({linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                       linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE});
}

TEST_CASE("Board::State::get_in_a_row_same_color_of_four")
{

    Board::State state{};
    auto test = Engine::Color::NONE;

    // Horizontal tests

    // [0, 0, 0, 0, 0, 0, 0]
    // [0, 0, 0, 0, 0, 0, 0]
    // [0, 0, 0, 0, 0, 0, 0]
    // [0, 0, 0, 0, 0, 0, 0]
    // [0, 0, 0, 0, 0, 0, 0]
    // [2, 2, 2, 2, 0, 0, 0]

    state.layout[1][0] = Engine::Color::BLUE;
    state.layout[1][1] = Engine::Color::BLUE;
    state.layout[1][2] = Engine::Color::BLUE;
    state.layout[1][3] = Engine::Color::BLUE;

    test = state.get_in_a_row_same_color_of_four();

    CHECK(test == Engine::Color::BLUE);

    clear_board_state_layout(state);

    state.layout[0][0] = Engine::Color::RED;
    state.layout[0][1] = Engine::Color::RED;
    state.layout[0][2] = Engine::Color::RED;
    state.layout[0][3] = Engine::Color::RED;

    test = state.get_in_a_row_same_color_of_four();

    CHECK(test == Engine::Color::RED);

    clear_board_state_layout(state);

    // Vertical tests

    // [0, 0, 0, 0, 0, 0, 0]
    // [0, 0, 0, 0, 0, 0, 0]
    // [2, 0, 0, 0, 0, 0, 0]
    // [2, 0, 0, 0, 0, 0, 0]
    // [2, 0, 0, 0, 0, 0, 0]
    // [2, 0, 0, 0, 0, 0, 0]

    state.layout[0][0] = Engine::Color::RED;
    state.layout[1][0] = Engine::Color::RED;
    state.layout[2][0] = Engine::Color::RED;
    state.layout[3][0] = Engine::Color::RED;

    test = state.get_in_a_row_same_color_of_four();

    CHECK(test == Engine::Color::RED);

    clear_board_state_layout(state);

    state.layout[2][0] = Engine::Color::BLUE;
    state.layout[3][0] = Engine::Color::BLUE;
    state.layout[4][0] = Engine::Color::BLUE;
    state.layout[5][0] = Engine::Color::BLUE;

    test = state.get_in_a_row_same_color_of_four();

    CHECK(test == Engine::Color::BLUE);

    clear_board_state_layout(state);

    // NUL tests

    state.layout[0][0] = Engine::Color::RED;
    state.layout[1][0] = Engine::Color::BLUE;

    test = state.get_in_a_row_same_color_of_four();

    CHECK(test == Engine::Color::NONE);
}