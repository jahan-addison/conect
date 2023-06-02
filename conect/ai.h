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

#include <resource.h>

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