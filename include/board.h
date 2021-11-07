#pragma once

#include <engine.h>
#include <nanogui/canvas.h>
#include <nanogui/opengl.h>
#include <nanogui/renderpass.h>
#include <nanogui/screen.h>

#include <array>
#include <string_view>
#include <filesystem>
#include <utility>

#if defined(_WIN32)
#if defined(APIENTRY)
#undef APIENTRY
#endif
#include <windows.h>
#endif

namespace fjorir {

using nanogui::Canvas;
using nanogui::ref;
using nanogui::Vector2f;
using nanogui::Vector2i;

constexpr float Pi = 3.14159f;

class Board : public Canvas
{
public:
    Board(Widget* parent, Engine* engine)
        : Canvas(parent, 1), engine(engine), m_image(-1)
    {
    }
    virtual ~Board() {}
    virtual void draw(NVGcontext* ctx) override;

public:
    Engine* engine;
    using Image = int;

    struct resource
    {
        enum class Type
        {
            BOARD = 0,
            RED_COIN,
            BLUE_COIN
        };

        std::array<std::string_view, 2> const root = {
            "../../../resources/",
            "./resources/"
        };
        std::string_view const board = "fjorir-board-2.png";
        std::string_view const red_coin = "circle-red.png";
        std::string_view const blue_coin = "circle-blue.png";

        Board::Image load_resource(NVGcontext* ctx, Type type) const;
    };

    using resource_type = resource::Type;

private:
    inline std::pair<float, float> get_coin_drawing_pos(float x_pos,
        float y_pos) const noexcept;
    bool add_coin(NVGcontext* ctx, Engine::Column col, Engine::Color color);
    void draw_coins(NVGcontext* ctx) const;

private:
    Image m_image{};
    resource res{};
    std::array<std::array<Image, 6>, 7> m_layout{};
};

}  // namespace fjorir