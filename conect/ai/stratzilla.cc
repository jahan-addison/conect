/*
    The code below uses a modified version of the @stratzilla Alpha-beta
    pruning Minimax algorithm for connect-4. The license is here:
    https://github.com/stratzilla/connect-four/blob/master/LICENSE
*/

#include <ai/stratzilla.h>
#include <engine.h>
#include <iostream>
#include <vector>

namespace conect {

namespace AI {

using std::max;
using std::min;

constexpr int NUM_ROW = gui::Size::ROW;
constexpr int NUM_COL = gui::Size::COL;

void
board_move(gui::Layout* board, unsigned int column, gui::Color color)
{
    // start from bottom of board going up
    for (unsigned int r = 0; r < NUM_ROW; r++) {
        if ((*board)[r][column] == gui::Color::NONE) { // first available spot
            (*board)[r][column] = color;               // set piece
            break;
        }
    }
}

gui::Piece
ST::get_next_move(Difficulty d) const
{
    return { gui::Color::NONE, gui::Column::COL_E };
}

gui::Piece
ST::get_next_move_as_beginner_ai() const
{
    return { gui::Color::NONE, gui::Column::COL_E };
}

gui::Piece
ST::get_next_move_as_advanced_ai() const
{
    return { gui::Color::NONE, gui::Column::COL_E };
}

ST::move
ST::minimax_alpha_beta_pruning(gui::Layout* board,
                               int depth,
                               int alpha,
                               int beta,
                               gui::Color color)
{
    auto b = board;
    auto turns = engine_->get_turn_count();
    auto PLAYER = gui::Color::RED;
    auto COMPUTER = gui::Color::BLUE;
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
        return std::array<int, 2>{ tabulate_score(COMPUTER), -1 };
    }
    if (color == COMPUTER) { // if AI player
        std::array<int, 2> moveSoFar = {
            INT_MIN, -1
        }; // since maximizing, we want lowest possible value
        if (get_next_move_is_winning(PLAYER)) { // if player about to win
            return moveSoFar; // force it to say it's worst possible score, so
                              // it knows to avoid move
        }                     // otherwise, business as usual
        for (unsigned int c = 0; c < NUM_COL; c++) { // for each possible move
            if ((*b)[NUM_ROW - 1][c] == gui::Color::NONE) {
                auto newBoard = *b;
                board_move(&newBoard, c, color); // try the move
                int score = minimax_alpha_beta_pruning(
                  &newBoard,
                  depth - 1,
                  alpha,
                  beta,
                  PLAYER)[0]; // find move based on that new board state
                if (score > moveSoFar[0]) { // if better score, replace it, and
                    // consider that best move (for now)
                    moveSoFar = { score, (int)c };
                }
                alpha = max(alpha, moveSoFar[0]);
                if (alpha >= beta) {
                    break;
                } // for pruning
            }
        }
        return moveSoFar; // return best possible move
    } else {
        std::array<int, 2> moveSoFar = {
            INT_MAX, -1
        }; // since PLAYER is minimized, we want moves that diminish this score
        if (get_next_move_is_winning(COMPUTER)) {
            return moveSoFar; // if about to win, report that move as best
        }
        for (unsigned int c = 0; c < NUM_COL; c++) {
            if ((*b)[NUM_ROW - 1][c] == gui::Color::NONE) {
                auto newBoard = *b;
                board_move(&newBoard, c, color);
                int score = minimax_alpha_beta_pruning(
                  &newBoard, depth - 1, alpha, beta, COMPUTER)[0];
                if (score < moveSoFar[0]) {
                    moveSoFar = { score, (int)c };
                }
                beta = min(beta, moveSoFar[0]);
                if (alpha >= beta) {
                    break;
                }
            }
        }
        return moveSoFar;
    }
}

ST::score
ST::get_score_of_set(std::vector<gui::Color> const& set, gui::Color color)
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

bool
ST::get_next_move_is_winning(gui::Color color) const
{
    auto b = board_;
    unsigned int winSequence = 0; // to count adjacent pieces
    // for horizontal checks
    for (unsigned int c = 0; c < NUM_COL - 3; c++) { // for each column
        for (unsigned int r = 0; r < NUM_ROW; r++) { // each row
            for (int i = 0; i < 4; i++) {            // recall you need 4 to win
                if ((*b)[r][c + i] == color) {       // if not all pieces match
                    winSequence++;                   // add sequence count
                }
                if (winSequence == 4) {
                    return true;
                } // if 4 in row
            }
            winSequence = 0; // reset counter
        }
    }
    // vertical checks
    for (unsigned int c = 0; c < NUM_COL; c++) {
        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                if ((*b)[r + i][c] == color) {
                    winSequence++;
                }
                if (winSequence == 4) {
                    return true;
                }
            }
            winSequence = 0;
        }
    }
    // the below two are diagonal checks
    for (unsigned int c = 0; c < NUM_COL - 3; c++) {
        for (unsigned int r = 3; r < NUM_ROW; r++) {
            for (int i = 0; i < 4; i++) {
                if ((*b)[r - i][c + i] == color) {
                    winSequence++;
                }
                if (winSequence == 4) {
                    return true;
                }
            }
            winSequence = 0;
        }
    }
    for (unsigned int c = 0; c < NUM_COL - 3; c++) {
        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (int i = 0; i < 4; i++) {
                if ((*b)[r + i][c + i] == color) {
                    winSequence++;
                }
                if (winSequence == 4) {
                    return true;
                }
            }
            winSequence = 0;
        }
    }
    return false; // otherwise no winning move
}

ST::score
ST::tabulate_score(gui::Color color)
{
    score score{ 0 };
    auto b = board_;

    std::array<gui::Color, NUM_COL> rs{ gui::Color::NONE };
    std::array<gui::Color, NUM_ROW> cs{ gui::Color::NONE };

    std::vector<gui::Color> set(4);

    /**
     * horizontal checks, we're looking for sequences of 4
     * containing any combination of AI, PLAYER, and empty pieces
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
ST::heuristic_function(points g, points b, points z)
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
