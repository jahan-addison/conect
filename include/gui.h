#pragma once

#include <board.h>
#include <engine.h>
#include <nanogui/nanogui.h>
#include <memory>
#include <iostream>

namespace fjorir {

using namespace nanogui;

enum class difficulty : int { Beginner = 0, Hard, Insane };

class GUI : public Screen
{
public:
    explicit GUI(bool init = true);
    GUI(GUI& gui) = delete;
private:
    std::shared_ptr<Engine> engine;
    std::shared_ptr<Board> canvas;

    void set_board_actions();
    void set_sidebar();
    void set_board();
    difficulty default_difficulty = difficulty::Beginner;
};

}  // namespace fjorir