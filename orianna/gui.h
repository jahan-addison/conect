/*
 *   orianna is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <ai.h>
#include <memory>
#include <nanogui/nanogui.h>
#include <string>

#if defined(_WIN32)
#if defined(APIENTRY)
#undef APIENTRY
#endif
#include <windows.h>
#endif

namespace orianna {

class Engine;
class Board;

class GUI : public nanogui::Screen
{
  public:
    GUI();
    GUI(GUI& gui) = delete;

  private:
    std::shared_ptr<Engine> engine;
    std::shared_ptr<Board> canvas;
    void set_board_actions();

    void board_event(board::column column);

    void set_sidebar();
    void set_board();
};

} // namespace orianna