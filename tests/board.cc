#include "doctest/doctest.h"
#include <board.h>
#include <iostream>

using namespace linea;

inline void clear_board_state_layout(Board::State &state)
{
    state.layout.fill({linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                       linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE});
}

inline void fill_board_state_layout(Board::State &state)
{
    state.layout.fill({linea::Engine::Color::RED, linea::Engine::Color::RED, linea::Engine::Color::RED,
                       linea::Engine::Color::RED, linea::Engine::Color::RED, linea::Engine::Color::RED});
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

TEST_CASE("Board::State::is_won")
{

    Board::State state{};

    // 4 in a straight row test

    state.layout[1][0] = Engine::Color::BLUE;
    state.layout[1][1] = Engine::Color::BLUE;
    state.layout[1][2] = Engine::Color::BLUE;
    state.layout[1][3] = Engine::Color::BLUE;

    CHECK(state.is_won() == Engine::Color::BLUE);

    // 4 in a diagonal row test

    state.layout = {{{linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::BLUE, linea::Engine::Color::BLUE, linea::Engine::Color::BLUE,
                      linea::Engine::Color::RED, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::BLUE, linea::Engine::Color::BLUE, linea::Engine::Color::RED,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::BLUE, linea::Engine::Color::RED, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::RED, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE}}};

    CHECK(state.is_won() == Engine::Color::RED);
}

TEST_CASE("Board::State::is_full")
{

    Board::State state{};
    bool test{true};

    test = state.is_full();

    CHECK(test == false);

    fill_board_state_layout(state);

    test = state.is_full();

    CHECK(test == true);

    clear_board_state_layout(state);
}

TEST_CASE("Board::State::get_in_a_row_same_color_of_four")
{

    Board::State state{};
    auto test = Engine::Color::NONE;

    clear_board_state_layout(state);
    test = state.get_diagonal_same_color_of_four(true);

    CHECK(test == Engine::Color::NONE);

    // forward diagonal tests

    // [0, 0, 0, 0, 0, 0, 0]
    // [0, 0, 0, 0, 0, 0, 0]
    // [0, 0, 0, 2, 0, 0, 0]
    // [0, 0, 2, 1, 0, 0, 0]
    // [0, 2, 1, 1, 0, 0, 0]
    // [2, 1, 1, 1, 0, 0, 0]

    state.layout = {{{linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::BLUE, linea::Engine::Color::BLUE, linea::Engine::Color::BLUE,
                      linea::Engine::Color::RED, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::BLUE, linea::Engine::Color::BLUE, linea::Engine::Color::RED,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::BLUE, linea::Engine::Color::RED, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::RED, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE}}};

    test = state.get_diagonal_same_color_of_four(true);

    CHECK(test == Engine::Color::RED);

    // backward diagonal tests

    // [0, 0, 0, 0, 0, 0, 0]
    // [0, 0, 0, 0, 0, 0, 0]
    // [0, 0, 0, 2, 0, 0, 0]
    // [0, 0, 0, 1, 2, 0, 0]
    // [0, 0, 0, 1, 1, 2, 0]
    // [0, 0, 0, 1, 1, 1, 2]

    test = Engine::Color::NONE;

    state.layout = {{{linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::RED,
                      linea::Engine::Color::BLUE, linea::Engine::Color::BLUE, linea::Engine::Color::BLUE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::RED, linea::Engine::Color::BLUE, linea::Engine::Color::BLUE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::RED, linea::Engine::Color::BLUE},
                     {linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::NONE,
                      linea::Engine::Color::NONE, linea::Engine::Color::NONE, linea::Engine::Color::RED}}};

    test = state.get_diagonal_same_color_of_four(false);

    CHECK(test == Engine::Color::RED);
}