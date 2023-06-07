/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <common.h>

namespace conect {

class Engine;

namespace AI {

enum class Difficulty : int
{
    Beginner,
    Hard
};

class IAlgorithm
{
  public:
    virtual gui::Piece get_next_move(Difficulty d) const = 0;
    virtual gui::Piece get_next_move_as_beginner_ai() const = 0;
    virtual gui::Piece get_next_move_as_advanced_ai() const = 0;

    virtual bool get_next_move_is_winning(gui::Color color) const = 0;

  private:
    virtual int tabulate_score(gui::Color color) = 0;
};

} // namespace AI
} // namespace conect