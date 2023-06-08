/*
    The code below uses a modified version of the @stratzilla Alpha-beta
    pruning Minimax algorithm for connect-4. The license is here:
    https://github.com/stratzilla/connect-four/blob/master/LICENSE
*/

#include <ai.h>
#include <array>
#include <memory>
#include <vector>

namespace conect {

class Engine;

namespace AI {

class Stratzilla_Algorithm;

using ST = Stratzilla_Algorithm;

class Stratzilla_Algorithm final : public IAlgorithm
{
  public:
    explicit Stratzilla_Algorithm(std::shared_ptr<Engine> engine,
                                  gui::Layout* board)
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
    bool get_next_move_is_winning(gui::Color) const override;

    //  private:
    score tabulate_score(gui::Color c) override;

    //  private:
    move minimax_alpha_beta_pruning(gui::Layout* board,
                                    int depth,
                                    int alpha,
                                    int beta,
                                    gui::Color color);
    score heuristic_function(points good, points bad, points empty);

    score get_score_of_set(std::vector<gui::Color> const& set,
                           gui::Color color);

    //  private:
    std::shared_ptr<Engine> engine_;
    gui::Layout* board_;
};

} // namespace AI

} // namespace conect