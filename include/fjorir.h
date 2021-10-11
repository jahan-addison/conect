#pragma once

#include <nanogui/opengl.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/tabwidget.h>
#include <nanogui/texture.h>
#include <nanogui/nanogui.h> 
#include <iostream>
#include <memory>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#if defined(_MSC_VER)
#  pragma warning (disable: 4505) // don't warn about dead code in stb_image.h
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include <gltexture.h>

namespace fjorir {
    using namespace nanogui;

    enum class difficulty {
        Beginner = 0,
        Hard,
        Insane
    };

    class Engine : public Screen
    {
    public:
        explicit Engine(bool init = true);
        Engine(Engine& Engine) = delete;

    private:
        void set_board_actions();
        void set_sidebar();
        void set_game_board();
        using ImageHolder = std::unique_ptr<uint8_t[], void(*)(void*)>;
        std::vector<std::pair<ref<Texture>, ImageHolder>> m_images;
        difficulty default_difficulty = difficulty::Beginner;
    };
} // namespace fjorir