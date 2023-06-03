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
Engine::get_player(Players p)
{
    switch (p) {
        case Engine::Players::First:
            return &player_1;
        case Engine::Players::Second:
            return &player_2;
        default:
            return &player_1;
    }
}

std::optional<Engine::Player>
Engine::is_won() const
{
    // auto color = this->get_diagonal_same_color_of_four();
    // if (color == Engine::Color::NONE)
    //     color = this->get_diagonal_same_color_of_four(false);
    // if (color == Engine::Color::NONE)
    //     color = this->get_in_a_row_same_color_of_four();
    return std::nullopt;
}

bool
Engine::is_full(resource::Layout layout) const
{
    bool full = true;
    for (auto const& k : layout) {
        for (auto const& j : k)
            if (j != resource::Color::RED and j != resource::Color::BLUE)
                full = false;
    }
    return full;
}

} // namespace conect