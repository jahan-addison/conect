/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <ai.h>
#include <board.h>
#include <optional>
#include <common.h>
#include <string>
#include <string_view>
#include <utility>

namespace conect {

class Engine
{
  public:
    Engine(Engine& engine) = delete;
    Engine(AI::Difficulty d = AI::Difficulty::Beginner)
      : difficulty(d)
    {
    }

    enum class Players
    {
        First,
        Second
    };

  private:
    struct Player
    {
        Player() = delete;
        explicit Player(std::string name_, resource::Color color_, bool ai_)
          : name(std::move(name_))
          , color(color_)
          , ai(ai_)
        {
        }
        std::string name;
        resource::Color color;
        bool ai;

        friend inline bool operator==(Player const& l, Player const& r)
        {
            return &l == &r;
        }

        friend inline bool operator==(Player const& l, resource::Color const& r)
        {
            return l.color == r;
        }

        friend inline bool operator!=(auto const& l, auto const& r)
        {
            return !(l == r);
        }
    };

  public:
    constexpr void set_current_difficulty(AI::Difficulty d) { difficulty = d; }

    constexpr AI::Difficulty get_current_difficulty() { return difficulty; }

    void set_next_player();
    Player* get_player(Players p);
    bool is_full(resource::Layout layout) const;
    std::optional<Player> is_won() const;

    inline Player* get_current_player() const { return player; }
    inline void set_player_name(Player* p, std::string& name)
    {
        p->name = std::move(name);
    }

  private:
    Player player_1{ "Anonymous", resource::Color::RED, false };
    Player player_2{ "AI", resource::Color::BLUE, true };
    Player* player = &player_1;
    AI::Difficulty difficulty;
};

} // namespace conect