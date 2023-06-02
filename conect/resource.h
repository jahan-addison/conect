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

#pragma once

#include <array>
#include <string_view>

namespace conect {

namespace resource {

enum class Color : int
{
    RED = 2,
    BLUE,
    NONE
};

enum class Resource_Type
{
    BOARD = 0,
    RED_COIN,
    BLUE_COIN
};

enum class Column : int
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

using Piece = std::pair<Color, Column>;

enum Size : size_t
{
    ROW = 6,
    COL = 7
};

using Image = int;
using Layout = std::array<std::array<Color, Size::ROW>, Size::COL>;
using Resource_Layout = std::array<std::array<Image, Size::ROW>, Size::COL>;

constexpr std::string_view root_resource_dir = "./resources/";
constexpr std::string_view board_resource = "conect-board-2.png";
constexpr std::string_view red_coin_resource = "circle-red.png";
constexpr std::string_view blue_coin_resource = "circle-blue.png";

} // namespace resource

} // namespace conect
