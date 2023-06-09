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

namespace ai {

class Stratzilla_Algorithm;

using ST = Stratzilla_Algorithm;

class Stratzilla_Algorithm final : public IAlgorithm
{
  public:
    explicit Stratzilla_Algorithm(std::shared_ptr<Engine> engine,
                                  board::layout* board)
      : engine_(engine)
      , board_(board)
    {
    }

    using score = int;
    using move = std::array<int, 2>;
    using points = unsigned int;

  public:
    int get_next_move(difficulty d) const override;

    inline int get_next_move_as_beginner_ai() const override;
    inline int get_next_move_as_advanced_ai() const override;
    bool get_next_move_is_winning(board::color) const override;

    //  private:
    score tabulate_score(board::color c) const override;

    //  private:
    move minimax_alpha_beta_pruning(board::layout* board,
                                    int depth,
                                    int alpha,
                                    int beta,
                                    board::color color) const;
    score heuristic_function(points good, points bad, points empty) const;

    score get_score_of_set(std::vector<board::color> const& set,
                           board::color color) const;

    //  private:
    std::shared_ptr<Engine> engine_;
    board::layout* board_;
};

} // namespace ai

} // namespace conect