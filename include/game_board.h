#pragma once

#include <nanogui/canvas.h>
#include <nanogui/shader.h>
#include <nanogui/renderpass.h>
#include <GLFW/glfw3.h>
#include <iostream>

#if defined(_WIN32)
#  if defined(APIENTRY)
#    undef APIENTRY
#  endif
#  include <windows.h>
#endif

namespace fjorir {

    using nanogui::Vector3f;
    using nanogui::Vector2i;
    using nanogui::Shader;
    using nanogui::Canvas;
    using nanogui::ref;


    constexpr float Pi = 3.14159f;

    class Game_Board : public Canvas {
    public:
        Game_Board(Widget* parent);
        void set_rotation(float rotation);
        virtual void draw_contents() override;
    private:
        ref<Shader> m_shader;
        float m_rotation;
    };
} // namespace fjorir