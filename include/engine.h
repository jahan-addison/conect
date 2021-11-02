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
        enum class COLUMN : int {
            COL_1 = 0,
            COL_2,
            COL_3,
            COL_4,
            COL_5,
            COL_6,
            COL_7,
            // unknown:
            COL_Z
        };

        constexpr bool get_is_open() const noexcept {
            return is_open;
        }

        constexpr std::string_view column_to_name(COLUMN col) const noexcept {
            switch (col) {
            case COLUMN::COL_1: return "column 1"; break;
            case COLUMN::COL_2: return "column 2"; break;
            case COLUMN::COL_3: return "column 3"; break;
            case COLUMN::COL_4: return "column 4"; break;
            case COLUMN::COL_5: return "column 5"; break;
            case COLUMN::COL_6: return "column 6"; break;
            case COLUMN::COL_7: return "column 7"; break;

            default: return "unknown column";
            }
        }

        inline COLUMN last() const noexcept {
            return action_queue.empty() ? COLUMN::COL_Z : action_queue.front();
        }

        inline void add_coin(COLUMN col) {
            is_open = true;
            action_queue.emplace_front(col);
        }

        COLUMN pop_coin() noexcept {
            auto coin = action_queue.front();
            action_queue.pop_front();
            is_open = false;
            return coin;
        }

    private:
        std::forward_list <COLUMN> action_queue{};
        bool is_open{ false };
    };
} // namespace fjorir