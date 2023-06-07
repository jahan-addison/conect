/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <array>
#include <string_view>

namespace conect {

namespace gui {

constexpr std::string_view root_resource_dir = "./resources/";
constexpr std::string_view board_resource = "conect-board-2.png";
constexpr std::string_view red_coin_resource = "circle-red.png";
constexpr std::string_view blue_coin_resource = "circle-blue.png";

using Image = int;

enum Size : size_t
{
    ROW = 6,
    COL = 7
};

enum class Color : unsigned int
{
    RED = 2,
    BLUE,
    NONE
};

enum class Column : unsigned int
{
    COL_E,
    COL_1,
    COL_2,
    COL_3,
    COL_4,
    COL_5,
    COL_6,
    COL_7,
};

enum class Resource_Type
{
    BOARD = 0,
    RED_COIN,
    BLUE_COIN
};

template<typename T>
std::ostream&
operator<<(
  typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream,
  const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

constexpr std::string_view
column_to_string(gui::Column col)
{
    using Column = gui::Column;
    switch (col) {
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

using Piece = std::pair<Color, Column>;

using Layout = std::array<std::array<Color, Size::ROW>, Size::COL>;
using Resource_Layout = std::array<std::array<Image, Size::ROW>, Size::COL>;

} // namespace gui

} // namespace conect
