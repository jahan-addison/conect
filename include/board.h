#pragma once

#include <nanogui/canvas.h>
#include <nanogui/shader.h>
#include <nanogui/screen.h>
#include <nanogui/opengl.h>
#include <nanogui/renderpass.h>
#include <GLFW/glfw3.h>
#include <engine.h>
#include <utility>
#include <array>

#if defined(_WIN32)
#  if defined(APIENTRY)
#    undef APIENTRY
#  endif
#  include <windows.h>
#endif

namespace fjorir {

    using nanogui::Vector3f;
    using nanogui::Vector2i;
    using nanogui::Matrix4f;
    using nanogui::Vector2f;
    using nanogui::Vector2i;
    using nanogui::Shader;
    using nanogui::Canvas;
    using nanogui::ref;


    constexpr float Pi = 3.14159f;

    class Board : public Canvas {
    public:

        Board(Widget* parent, Engine* engine);
        virtual ~Board();
        constexpr void set_rotation(float rotation);
        virtual void draw_contents() override;
        virtual void draw(NVGcontext* ctx) override;
    public:
        Engine* engine;
    private:
        std::pair<float, float> get_coin_drawing_pos(int x_pos, int y_pos) const;
        bool add_coin(NVGcontext* ctx, Engine::Column col);
        void draw_coins(NVGcontext* ctx);
    private:
        using Image = int;
        ref<Shader> m_shader;
        int m_image;
        std::array<std::array < Image, 7>, 6> m_layout{};
        float m_rotation;
    };

} // namespace fjorir