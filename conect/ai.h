/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <common.h>

namespace conect {

class Engine;

namespace ai {

enum class difficulty : int
{
    beginner,
    hard
};

class IAlgorithm
{
  public:
    virtual int get_next_move(difficulty d) const = 0;
    virtual int get_next_move_as_beginner_ai() const = 0;
    virtual int get_next_move_as_advanced_ai() const = 0;

    virtual bool get_next_move_is_winning(board::color color) const = 0;

  private:
    virtual int tabulate_score(board::color color) const = 0;
};

} // namespace ai
} // namespace conect