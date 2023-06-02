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

#include <ai.h>
#include <board.h>
#include <optional>
#include <resource.h>
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
    Player get_player(Players player) const;
    bool is_full(resource::Layout layout) const;
    std::optional<Players> is_won() const;

    inline Player get_current_player() const { return *player; }
    inline void set_player_name(Player& player, std::string& name)
    {
        player.name = std::move(name);
    }

  private:
    Player player_1{ "Anonymous", resource::Color::RED, false };
    Player player_2{ "AI", resource::Color::BLUE, true };
    Player* player = &player_1;
    AI::Difficulty difficulty;
};

} // namespace conect