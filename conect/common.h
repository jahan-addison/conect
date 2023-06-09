/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#define BOARD_RESOURCE_FILE "conect-board.png"
#define BOARD_RED_COIN_RESOURCE "circle-red.png"
#define BOARD_BLUE_COIN_RESOURCE "circle-blue.png"

#include <array>
#include <ostream>
#include <string_view>

namespace conect {

namespace board {

constexpr std::string_view root_resource_directory = "./resources/";

template<typename T>
std::ostream&
operator<<(
  typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream,
  const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

enum size : size_t
{
    row = 6,
    col = 7
};

enum class color : int
{
    red = 2,
    blue,
    none
};

using column = unsigned int;

using layout = std::array<std::array<color, size::col>, size::row>;

using piece = std::pair<board::color, board::column>;

} // namespace board

namespace resource {

using image = int;

enum class type
{
    board = 0,
    red_coin,
    blue_coin
};

using layout =
  std::array<std::array<image, board::size::col>, board::size::row>;

} // namespace resource

} // namespace conect
