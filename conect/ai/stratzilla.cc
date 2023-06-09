/*
    The code below uses a modified version of the @stratzilla Alpha-beta
    pruning Minimax algorithm for connect-4. The license is here:
    https://github.com/stratzilla/connect-four/blob/master/LICENSE
*/

#include <ai/stratzilla.h>
#include <engine.h>
#include <vector>

namespace conect {

namespace ai {

using std::max;
using std::min;

constexpr int NUM_ROW = board::size::row;
constexpr int NUM_COL = board::size::col;

void
board_move(board::layout* board, int column, board::color color)
{
    for (int r = 0; r < NUM_ROW; r++) {
        if ((*board)[r][column] == board::color::none) { // first available spot
            (*board)[r][column] = color;                 // set piece
            break;
        }
    }
}

int
ST::get_next_move(difficulty d) const
{
    switch (d) {
        case difficulty::beginner:
            return get_next_move_as_beginner_ai();

        case difficulty::hard:
            return get_next_move_as_advanced_ai();

        default:
            return get_next_move_as_beginner_ai();
    }
}

inline int
ST::get_next_move_as_beginner_ai() const
{
    return minimax_alpha_beta_pruning(
      board_, 5, 0 - INT_MAX, INT_MAX, board::color::blue)[1];
}

inline int
ST::get_next_move_as_advanced_ai() const
{
    return minimax_alpha_beta_pruning(
      board_, 12, 0 - INT_MAX, INT_MAX, board::color::blue)[1];
}

ST::move
ST::minimax_alpha_beta_pruning(board::layout* board,
                               int depth,
                               int alpha,
                               int beta,
                               board::color color) const
{
    auto b = board;
    auto turns = engine_->get_turn_count();
    auto player = engine_->get_player(Engine::players::first)->color;
    auto computer = engine_->get_player(Engine::players::second)->color;
    /**
     * if we've reached minimal depth allowed by the program
     * we need to stop, so force it to return current values
     * since a move will never (theoretically) get this deep,
     * the column doesn't matter (-1) but we're more interested
     * in the score
     *
     * as well, we need to take into consideration how many moves
     * ie when the board is full
     */
    if (depth == 0 || depth >= (NUM_COL * NUM_ROW) - turns) {
        // get current score to return
        return std::array<int, 2>{ tabulate_score(computer), -1 };
    }
    if (color == computer) { // if ai player
        std::array<int, 2> best_move = {
            INT_MIN, -1
        }; // since maximizing, we want lowest possible value
        if (get_next_move_is_winning(player)) { // if player about to win
            return best_move; // force it to say it's worst possible score, so
                              // it knows to avoid move
        }                     // otherwise, business as usual
        for (unsigned int c = 0; c < NUM_COL; c++) { // for each possible move
            if ((*b)[NUM_ROW - 1][c] == board::color::none) {
                auto new_board = *b;
                board_move(&new_board, c, color); // try the move
                int score = minimax_alpha_beta_pruning(
                  &new_board,
                  depth - 1,
                  alpha,
                  beta,
                  player)[0]; // find move based on that new board state
                if (score > best_move[0]) { // if better score, replace it, and
                    // consider that best move (for now)
                    best_move = { score, (int)c };
                }
                alpha = max(alpha, best_move[0]);
                if (alpha >= beta) {
                    break;
                } // for pruning
            }
        }
        return best_move; // return best possible move
    } else {
        std::array<int, 2> best_move = {
            INT_MAX, -1
        }; // since player is minimized, we want moves that diminish this score
        if (get_next_move_is_winning(computer)) {
            return best_move; // if about to win, report that move as best
        }
        for (unsigned int c = 0; c < NUM_COL; c++) {
            if ((*b)[NUM_ROW - 1][c] == board::color::none) {
                auto new_board = *b;
                board_move(&new_board, c, color);
                int score = minimax_alpha_beta_pruning(
                  &new_board, depth - 1, alpha, beta, computer)[0];
                if (score < best_move[0]) {
                    best_move = { score, (int)c };
                }
                beta = min(beta, best_move[0]);
                if (alpha >= beta) {
                    break;
                }
            }
        }
        return best_move;
    }
}

ST::score
ST::get_score_of_set(std::vector<board::color> const& set,
                     board::color color) const
{
    points good = 0;  // points in favor of player
    points bad = 0;   // points against player
    points empty = 0; // neutral spots
    for (points i = 0; i < set.size(); i++) {
        good += (set[i] == color) ? 1 : 0;
        bad +=
          (set[i] == board::color::blue or set[i] == board::color::red) ? 1 : 0;
        empty += (set[i] == board::color::none) ? 1 : 0;
    }
    // bad was calculated as (bad + good), so remove good
    bad -= good;
    return heuristic_function(good, bad, empty);
}

bool
ST::get_next_move_is_winning(board::color color) const
{
    auto b = board_;
    unsigned int win_sequence = 0; // to count adjacent pieces
    // for horizontal checks
    for (unsigned int c = 0; c < NUM_COL - 3; c++) { // for each column
        for (unsigned int r = 0; r < NUM_ROW; r++) { // each row
            for (int i = 0; i < 4; i++) {            // recall you need 4 to win
                if ((*b)[r][c + i] == color) {       // if not all pieces match
                    win_sequence++;                  // add sequence count
                }
                if (win_sequence == 4) {
                    return true;
                } // if 4 in row
            }
            win_sequence = 0; // reset counter
        }
    }
    // vertical checks
    for (unsigned int c = 0; c < NUM_COL; c++) {
        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                if ((*b)[r + i][c] == color) {
                    win_sequence++;
                }
                if (win_sequence == 4) {
                    return true;
                }
            }
            win_sequence = 0;
        }
    }
    // the below two are diagonal checks
    for (unsigned int c = 0; c < NUM_COL - 3; c++) {
        for (unsigned int r = 3; r < NUM_ROW; r++) {
            for (int i = 0; i < 4; i++) {
                if ((*b)[r - i][c + i] == color) {
                    win_sequence++;
                }
                if (win_sequence == 4) {
                    return true;
                }
            }
            win_sequence = 0;
        }
    }
    for (unsigned int c = 0; c < NUM_COL - 3; c++) {
        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                if ((*b)[r + i][c + i] == color) {
                    win_sequence++;
                }
                if (win_sequence == 4) {
                    return true;
                }
            }
            win_sequence = 0;
        }
    }
    return false; // otherwise no winning move
}

ST::score
ST::tabulate_score(board::color color) const
{
    score score{ 0 };
    auto b = board_;

    std::array<board::color, NUM_COL> rs{ board::color::none };
    std::array<board::color, NUM_ROW> cs{ board::color::none };

    std::vector<board::color> set(4);

    /**
     * horizontal checks, we're looking for sequences of 4
     * containing any combination of ai, player, and empty pieces
     */
    for (unsigned int r = 0; r < NUM_ROW; r++) {
        for (unsigned int c = 0; c < NUM_COL; c++) {
            rs[c] = (*b)[r][c]; // this is a distinct row alone
        }
        for (unsigned int c = 0; c < NUM_COL - 3; c++) {
            for (int i = 0; i < 4; i++) {
                set[i] =
                  rs[c + i]; // for each possible "set" of 4 spots from that row
            }
            score += get_score_of_set(set, color); // find score
        }
    }

    // vertical
    for (unsigned int c = 0; c < NUM_COL; c++) {
        for (unsigned int r = 0; r < NUM_ROW; r++) {
            cs[r] = (*b)[r][c];
        }
        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                set[i] = cs[r + i];
            }
            score += get_score_of_set(set, color); // find score
        }
    }

    // diagonals
    for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
        for (unsigned int c = 0; c < NUM_COL; c++) {
            rs[c] = (*b)[r][c];
        }
        for (unsigned int c = 0; c < NUM_COL - 3; c++) {
            for (int i = 0; i < 4; i++) {
                set[i] = (*b)[r + i][c + i];
            }
            score += get_score_of_set(set, color); // find score
        }
    }
    for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
        for (unsigned int c = 0; c < NUM_COL; c++) {
            rs[c] = (*b)[r][c];
        }
        for (unsigned int c = 0; c < NUM_COL - 3; c++) {
            for (int i = 0; i < 4; i++) {
                set[i] = (*b)[r + 3 - i][c + i];
            }
            score += get_score_of_set(set, color); // find score
        }
    }

    return score;
}

ST::score
ST::heuristic_function(points g, points b, points z) const
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
