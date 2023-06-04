/*
    The code below uses a modified version of the @stratzilla Alpha-beta
    pruning Minimax algorithm for connect-4. The license is here:
    https://github.com/stratzilla/connect-four/blob/master/LICENSE
*/

#include <ai/stratzilla.h>
#include <vector>

namespace conect {

namespace AI {

Stratzilla_Algorithm::score
Stratzilla_Algorithm::tabulate_score()
{
    // score s{ 0 };
    // std::array<int, gui::Size::COL> rs{};
    // std::array<int, gui::Size::ROW> cs{};
    // std::vector<unsigned int> set{ 4 };
    return 0;
}

Stratzilla_Algorithm::score
Stratzilla_Algorithm::get_score_of_set(
  std::array<gui::Color, gui::Size::COL> set,
  gui::Color color)
{
    points good = 0;  // points in favor of player
    points bad = 0;   // points against player
    points empty = 0; // neutral spots
    for (points i = 0; i < set.size(); i++) {
        good += (set[i] == color) ? 1 : 0;
        bad +=
          (set[i] == gui::Color::BLUE or set[i] == gui::Color::RED) ? 1 : 0;
        empty += (set[i] == gui::Color::NONE) ? 1 : 0;
    }
    // bad was calculated as (bad + good), so remove good
    bad -= good;
    return heuristic_function(good, bad, empty);
}

Stratzilla_Algorithm::score
Stratzilla_Algorithm::heuristic_function(points g, points b, points z)
{
    score score = 0;
    if (g == 4) {
        score += 500001;
    } // preference to go for winning move vs. block
    else if (g == 3 and z == 1) {
        score += 5000;
    } else if (g == 2 and z == 2) {
        score += 500;
    } else if (b == 2 and z == 2) {
        score -= 501;
    } // preference to block
    else if (b == 3 and z == 1) {
        score -= 5001;
    } // preference to block
    else if (b == 4) {
        score -= 500000;
    }
    return score;
}

} // namespace ai
} // namespace conect
