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

class Algorithm
{
  private:
    virtual resource::Piece get_next_move(Difficulty d) const = 0;

    virtual resource::Piece get_next_move_as_beginner_ai() const = 0;
    virtual resource::Piece get_next_move_as_advanced_ai() const = 0;

  public:
    int tabulate_score(resource::Layout layout, Engine engine);
    int tabulate_score_diagonal(resource::Layout layout, Engine engine);
    int tabulate_score_horizontal(resource::Layout layout, Engine engine);
    int tabulate_score_vertical(resource::Layout layout, Engine engine);
};

} // namespace AI
} // namespace conect