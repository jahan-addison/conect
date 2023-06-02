/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

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
    void on_coin_event(int index);
    void set_sidebar();
    void set_board();
};

} // namespace conect