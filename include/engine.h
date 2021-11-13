#pragma once

#include <board.h>
#include <nanogui/nanogui.h>
#include <forward_list>
#include <string_view>

namespace fjorir {

class Engine
{
public:
    explicit Engine() {}
    Engine(Engine& engine) = delete;

public:
    enum class Color : int
    {
        RED = 2,
        BLUE
    };

    inline friend std::ostream& operator << (std::ostream& os, Color const& obj)
    {
        auto j = static_cast<std::underlying_type<Color>::type>(obj);
        switch (j)
        {
        case 0: os << 0;
            break;
        case 2: os << "Red";
            break;
        case 3: os << "Blue";
            break;
        }
        return os;
    }

    enum class Column : int
    {
        COL_E = 0,
        COL_1,
        COL_2,
        COL_3,
        COL_4,
        COL_5,
        COL_6,
        COL_7,
    };

    constexpr bool get_is_receiving() const noexcept { return is_receiving; }

    constexpr std::string_view column_to_string(Column col) const noexcept
    {
        switch (col)
        {
        case Column::COL_E:
            return "none";
        case Column::COL_1:
            return "column 1";
        case Column::COL_2:
            return "column 2";
        case Column::COL_3:
            return "column 3";
        case Column::COL_4:
            return "column 4";
        case Column::COL_5:
            return "column 5";
        case Column::COL_6:
            return "column 6";
        case Column::COL_7:
            return "column 7";

        default:
            return "unknown column";
        }
    }

    constexpr int column_to_int(Column col) const noexcept
    {
        switch (col)
        {
        case Column::COL_1:
            return 0;
        case Column::COL_2:
            return 1;
        case Column::COL_3:
            return 2;
        case Column::COL_4:
            return 3;
        case Column::COL_5:
            return 4;
        case Column::COL_6:
            return 5;
        case Column::COL_7:
            return 6;
        default:
            return 0;
        }
    }

    inline void add_coin(Column col)
    {
        is_receiving = true;
        action_queue.emplace_front(is_red ? Color::RED : Color::BLUE,
            col);
    }

    std::pair<Color, Column> pop_coin() noexcept
    {
        auto coin = action_queue.front();
        action_queue.pop_front();
        is_receiving = false;
        is_red = !is_red;
        return coin;
    }

private:
    std::forward_list<std::pair<Color, Column>> action_queue{};
    bool is_red{ false };
    bool is_receiving{ false };
};
}  // namespace fjorir