#pragma once

#include <engine.h>
#include <nanogui/canvas.h>
#include <nanogui/opengl.h>
#include <nanogui/renderpass.h>
#include <nanogui/screen.h>

#include <array>
#include <filesystem>
#include <memory>
#include <string_view>
#include <utility>

#if defined(_WIN32)
#if defined(APIENTRY)
#undef APIENTRY
#endif
#include <windows.h>
#endif

namespace linea
{

using nanogui::Canvas;
using nanogui::ref;
using nanogui::Vector2f;
using nanogui::Vector2i;

constexpr float Pi = 3.14159f;

class Board : public Canvas
{
  public:
    Board(Widget *parent, std::shared_ptr<Engine> engine) : Canvas(parent, 1), engine(engine), m_image(-1)
    {
    }
    virtual void draw(NVGcontext *ctx) override;

  public:
    std::shared_ptr<Engine> engine;
    using Image = int;

    struct State
    {
        std::array<std::array<Engine::Color, 6>, 7> layout{};
        State() = default;
        State(State &state) = delete;
        Engine::Color is_won();
        bool is_full() const;

      public:
        Engine::Color get_diagonal_same_color_of_four(bool start_left) const;
        Engine::Color get_in_a_row_same_color_of_four() const;
    };

    struct Resource
    {
        enum class Type
        {
            BOARD = 0,
            RED_COIN,
            BLUE_COIN
        };

        std::array<std::string_view, 2> const file_roots = {"../../../resources/", "./resources/"};
        std::string_view const board = "linea-board-2.png";
        std::string_view const red_coin = "circle-red.png";
        std::string_view const blue_coin = "circle-blue.png";

        Board::Image load_resource(NVGcontext *ctx, Type type) const;
    };

    using resource_type = Resource::Type;

  private:
    inline std::pair<float, float> get_coin_drawing_pos(float x_pos, float y_pos) const noexcept;
    bool add_coin(NVGcontext *ctx, Engine::Column col, Engine::Color color);
    void draw_coins(NVGcontext *ctx) const;

  private:
    State state{};
    Image m_image{};
    Resource res{};
    std::array<std::array<Image, 6>, 7> m_layout{};
};

} // namespace linea