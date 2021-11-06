#pragma once

#include <engine.h>
#include <nanogui/canvas.h>
#include <nanogui/opengl.h>
#include <nanogui/renderpass.h>
#include <nanogui/screen.h>

#include <array>
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

    struct resources {};

private:
    inline std::pair<float, float> get_coin_drawing_pos(int x_pos,
        int y_pos) const noexcept;
    bool add_coin(NVGcontext* ctx, Engine::Column col);
    void draw_coins(NVGcontext* ctx) const;

private:
    using Image = int;
    Image m_image;
    std::array<std::array<Image, 6>, 7> m_layout{};
};

}  // namespace fjorir