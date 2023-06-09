/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
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

namespace conect {

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

    template<class T>
    requires(std::is_base_of_v<ai::IAlgorithm, T>)
    void board_event(board::column column);
    void state_event(bool ending);

    void set_sidebar();
    void set_board();

  private:
    bool ended_{ false };
};

} // namespace conect