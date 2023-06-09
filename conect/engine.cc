/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include <engine.h>

namespace conect {

void
Engine::set_next_player()
{
    if (*player == player_1)
        player = &player_2;
    else
        player = &player_1;
}

Engine::Player*
Engine::get_player(players p)
{
    switch (p) {
        case Engine::players::first:
            return &player_1;
        case Engine::players::second:
            return &player_2;
        default:
            return &player_1;
    }
}

bool
Engine::is_full(board::layout layout) const
{
    bool full = true;
    for (auto const& k : layout) {
        for (auto const& j : k)
            if (j != board::color::red and j != board::color::blue)
                full = false;
    }
    return full;
}

} // namespace conect