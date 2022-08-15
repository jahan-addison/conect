#include <board.h>
#include <engine.h>

namespace linea
{

Engine::Color_Column Engine::get_next_coin_color_column_by_algorithm(Engine::Difficulty d) const noexcept
{
    return {Color::NONE, Column::COL_1};
}

Engine::Color_Column Engine::beginner_decision_algorithm() noexcept
{
    return {Color::NONE, Column::COL_1};
}

Engine::Color_Column Engine::hard_decision_algorithm() noexcept
{
    return {Color::NONE, Column::COL_1};
}

void Engine::add_coin(Engine::Column col, Engine::Color color) noexcept
{
    action_queue.emplace_front(color, col);
}

void Engine::clear_coins() noexcept
{
    action_queue.clear();
}

} // namespace linea