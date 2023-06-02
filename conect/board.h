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

#include <engine.h>
#include <memory>
#include <nanogui/canvas.h>
#include <nanogui/opengl.h>
#include <nanogui/renderpass.h>
#include <nanogui/screen.h>
#include <ostream>
#include <resource.h>
#include <string_view>
#include <utility>

#if defined(_WIN32)
#if defined(APIENTRY)
#undef APIENTRY
#endif
#include <windows.h>
#endif

namespace conect {

using nanogui::Canvas;

constexpr float Pi = 3.14159f;

class Engine;

class Board : public Canvas
{
  public:
    Board(Widget* parent, std::shared_ptr<Engine> engine)
      : Canvas(parent, 1)
      , engine(engine)
    {
    }
    virtual void draw(NVGcontext* ctx) override;

  public:
    resource::Image load_resource(NVGcontext* ctx,
                                  resource::Resource_Type type);

  private:
    constexpr std::string_view column_to_string(resource::Column col);
    constexpr int column_to_int(resource::Column col);

    inline friend std::ostream& operator<<(std::ostream& os,
                                           resource::Color const& obj)
    {
        auto j = static_cast<std::underlying_type<resource::Color>::type>(obj);
        switch (j) {
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
    resource::Resource_Layout i_layout{};
    resource::Image board{ -1 };
    inline std::pair<float, float> get_coin_drawing_pos(
      float x_pos,
      float y_pos) const noexcept;
    void clear_board(NVGcontext* ctx);
    bool add_coin(NVGcontext* ctx, resource::Column col, resource::Color color);
    void draw_coins(NVGcontext* ctx) const;

  public:
    resource::Layout layout{};
    std::shared_ptr<Engine> engine;
};

} // namespace conect