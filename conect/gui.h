/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <ai.h>
#include <board.h>
#include <engine.h>
#include <iostream>
#include <memory>
#include <nanogui/nanogui.h>
#include <optional>
#include <string>

namespace conect {

using namespace nanogui;

class GUI : public Screen
{
  public:
    GUI();
    GUI(GUI& gui) = delete;

  private:
    std::shared_ptr<Engine> engine;
    std::shared_ptr<Board> canvas;
    void set_board_actions();

    template<class T>
    requires(std::is_base_of_v<AI::IAlgorithm, T>)
    void board_move_event(int index);

    void set_sidebar();
    void set_board();
};

} // namespace conect