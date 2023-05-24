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

Engine::Player
Engine::get_player(Players player) const
{
    switch (player) {
        case Engine::Players::First:
            return player_1;
        case Engine::Players::Second:
            return player_2;
        default:
            return player_1;
    }
}

std::optional<Engine::Players>
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