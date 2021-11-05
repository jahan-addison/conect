#pragma once

#include <nanogui/nanogui.h> 
#include <forward_list>
#include <string_view>
#include <board.h>

namespace fjorir {

    class Engine
    {
    public:
        explicit Engine() {}
        Engine(Engine& engine) = delete;
    public:
        enum class Color : int {
            RED = 0,
            BLUE
        };
        enum class Column : int {
            COL_E = 0,
            COL_1,
            COL_2,
            COL_3,
            COL_4,
            COL_5,
            COL_6,
            COL_7,
        };

        constexpr bool get_is_open() const noexcept {
            return is_open;
        }

        constexpr std::string_view column_to_string(Column col) const noexcept {
            switch (col) {
            case Column::COL_E: return "none";
            case Column::COL_1: return "column 1";
            case Column::COL_2: return "column 2";
            case Column::COL_3: return "column 3";
            case Column::COL_4: return "column 4";
            case Column::COL_5: return "column 5";
            case Column::COL_6: return "column 6";
            case Column::COL_7: return "column 7";

            default: return "unknown column";
            }
        }

        constexpr int column_to_int(Column col) const noexcept {
            switch (col) {
            case Column::COL_1: return 0;
            case Column::COL_2: return 1;
            case Column::COL_3: return 2;
            case Column::COL_4: return 3;
            case Column::COL_5: return 4;
            case Column::COL_6: return 5;
            case Column::COL_7: return 6;
            default: return 0;
            }
        }

        inline Column last() const noexcept {
            return action_queue.empty() ? Column::COL_E : action_queue.front();
        }

        inline void add_coin(Column col) {
            is_open = true;
            action_queue.emplace_front(col);
        }

        Column pop_coin() noexcept {
            auto coin = action_queue.front();
            action_queue.pop_front();
            is_open = false;
            return coin;
        }

    private:
        // std::forward_list<std::pair<Color, COLUMN>> action_queue{};
        std::forward_list <Column> action_queue{};
        bool is_open{ false };
    };
} // namespace fjorir