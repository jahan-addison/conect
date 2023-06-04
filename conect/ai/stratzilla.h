/*
    The code below uses a modified version of the @stratzilla Alpha-beta
    pruning Minimax algorithm for connect-4. The license is here:
    https://github.com/stratzilla/connect-four/blob/master/LICENSE
*/

#include <ai.h>
#include <array>
#include <engine.h>
#include <memory>

namespace conect {

namespace AI {

using std::max;
using std::min;

class Stratzilla_Algorithm final : public IAlgorithm
{
  public:
    Stratzilla_Algorithm(std::shared_ptr<Engine> engine,
                         std::shared_ptr<gui::Layout> board)
      : engine_(engine)
      , board_(board)
    {
    }

    using score = int;
    using move = std::array<int, 2>;
    using points = unsigned int;

  public:
    gui::Piece get_next_move(Difficulty d) const override;

    gui::Piece get_next_move_as_beginner_ai() const override;
    gui::Piece get_next_move_as_advanced_ai() const override;
    gui::Piece get_next_move_is_winning() const override;

  private:
    score tabulate_score() override;
    score tabulate_score_diagonal() override;
    score tabulate_score_horizontal() override;
    score tabulate_score_vertical() override;

  private:
    move minimax_alpha_beta_pruning(unsigned int depth, int alpha, int beta);
    score heuristic_function(points good, points bad, points empty);
    // score get_score_of_set(std::array<int, gui::Size::ROW> set,
    //                        gui::Color color);
    score get_score_of_set(std::array<gui::Color, gui::Size::COL> set,
                           gui::Color color);

  private:
    std::shared_ptr<Engine> engine_;
    std::shared_ptr<gui::Layout> board_;
};

} // namespace AI

} // namespace conect