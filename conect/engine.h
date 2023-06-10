/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <ai.h>
#include <board.h>
#include <common.h>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace conect {

class Engine
{
  public:
    Engine(Engine& engine) = delete;
    Engine(ai::difficulty d = ai::difficulty::beginner)
      : difficulty(d)
    {
    }

    enum class players
    {
        first,
        second
    };

  private:
    struct Player
    {
        Player() = delete;
        explicit Player(std::string name_, board::color color_, bool ai_)
          : name(std::move(name_))
          , color(color_)
          , ai(ai_)
        {
        }
        std::string name;
        board::color color;
        bool ai;

        friend inline bool operator==(Player const& l, Player const& r)
        {
            return &l == &r;
        }

        friend inline bool operator==(Player const& l, board::color const& r)
        {
            return l.color == r;
        }

        friend inline bool operator!=(auto const& l, auto const& r)
        {
            return !(l == r);
        }
    };

  public:
    constexpr void set_current_difficulty(ai::difficulty d) { difficulty = d; }
    constexpr void increment_turn_count() { turns_++; }
    constexpr int get_turn_count() { return turns_; }
    constexpr void reset_turns() { turns_ = 0; }
    constexpr void reset_engine_state() { ended_ = false; }
    constexpr void end_engine_state() { ended_ = true; }
    constexpr bool get_engine_state() { return ended_; }
    constexpr ai::difficulty get_current_difficulty() { return difficulty; }

    template<class T, class... Args>
    requires(std::is_base_of_v<ai::IAlgorithm, T>)
    inline constexpr T ai_factory(Args... args)
    {
        return T(args...);
    }

    void set_next_player();
    Player* get_player(players p);
    bool is_full(board::layout layout) const;

    inline Player* get_current_player() const { return player; }
    inline void set_player_name(Player* p, std::string& name)
    {
        p->name = std::move(name);
    }

  private:
    Player player_1{ "Anonymous", board::color::red, false };
    Player player_2{ "AI", board::color::blue, true };
    int turns_{ 0 };
    Player* player = &player_1;
    ai::difficulty difficulty{};

  private:
    bool ended_{ false };
};

} // namespace conect