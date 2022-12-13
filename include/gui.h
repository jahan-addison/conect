#pragma once

#include <board.h>
#include <engine.h>
#include <iostream>
#include <memory>
#include <nanogui/nanogui.h>
#include <optional>
#include <string>

namespace orianna {

using namespace nanogui;

class GUI : public Screen
{
  public:
    explicit GUI();
    GUI(GUI& gui) = delete;

  private:
    struct Player
    {
        std::string name{ "Anonymous" };
        Engine::Color color{ Engine::Color::NONE };
        bool ai{ false };

        friend inline bool operator==(Player const& l, Player const& r)
        {
            return &l == &r;
        }

        friend inline bool operator==(Player const& l, Engine::Color const& r)
        {
            return l.color == r;
        }

        friend inline bool operator!=(auto const& l, auto const& r)
        {
            return !(l == r);
        }
    };

  private:
    std::shared_ptr<Engine> engine;
    std::shared_ptr<Board> canvas;
    std::optional<Player> winner;

    Player player_1{};
    Player player_2{};

    Player* current_player_turn = &player_1;

    void set_players();
    void set_next_player();
    void set_board_actions();
    void on_coin_event(int index);
    void set_sidebar();
    void set_board();
};

} // namespace orianna