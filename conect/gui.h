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