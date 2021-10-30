#pragma once

#include <nanogui/canvas.h>
#include <nanogui/shader.h>
#include <nanogui/screen.h>
#include <nanogui/opengl.h>
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
    using nanogui::Matrix4f;
    using nanogui::Vector2f;
    using nanogui::Vector2i;
    using nanogui::Shader;
    using nanogui::Canvas;
    using nanogui::ref;


    constexpr float Pi = 3.14159f;

    class Board : public Canvas {
    public:
        Board(Widget* parent);
        virtual ~Board();
        void set_rotation(float rotation);
        virtual void draw_contents() override;
        virtual void draw(NVGcontext* ctx) override;
    private:
        ref<Shader> m_shader;
        int m_image;
        std::vector<int> m_coins;
        float m_rotation;
    };

} // namespace fjorir