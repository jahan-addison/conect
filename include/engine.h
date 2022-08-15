#pragma once

#include <board.h>
#include <forward_list>
#include <nanogui/nanogui.h>
#include <string_view>
#include <utility>

namespace linea
{

class Engine
{
  public:
    explicit Engine()
    {
    }
    Engine(Engine &engine) = delete;

  public:
    enum class Color : int
    {
        RED = 2,
        BLUE,
        NONE
    };

    enum class Difficulty
    {
        Beginner,
        Hard
    };

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

    using Color_Column = std::pair<Color, Column>;

    inline friend std::ostream &operator<<(std::ostream &os, Color const &obj)
    {
        auto j = static_cast<std::underlying_type<Color>::type>(obj);
        switch (j)
        {
        case 0:
            os << 0;
            break;
        case 2:
            os << "Red";
            break;
        case 3:
            os << "Blue";
            break;
        case 4:
            os << "None";
        }
        return os;
    }

  public:
    void clear_coins() noexcept;
    void add_coin(Column col, Color color) noexcept;
    Color_Column get_next_coin_color_column_by_algorithm(Difficulty d) const noexcept;
    Color_Column beginner_decision_algorithm() noexcept;
    Color_Column hard_decision_algorithm() noexcept;

    constexpr std::string_view column_to_string(Column col) noexcept
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

    constexpr int column_to_int(Column col) noexcept
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

    constexpr void set_current_difficulty(Difficulty d) noexcept
    {
        algorithm_difficulty = d;
    }

    constexpr Difficulty get_current_difficulty() noexcept
    {
        return algorithm_difficulty;
    }

  private:
    std::forward_list<Color_Column> action_queue{};
    Difficulty algorithm_difficulty = Difficulty::Beginner;
};
} // namespace linea