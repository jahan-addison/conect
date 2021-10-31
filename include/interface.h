#pragma once

#include <nanogui/opengl.h>
#include <nanogui/tabwidget.h>
#include <nanogui/texture.h>
#include <nanogui/nanogui.h> 
#include <iostream>

#include <board.h>


#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#if defined(_MSC_VER)
#  pragma warning (disable: 4505) // don't warn about dead code in stb_image.h
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wunused-function"
#endif

namespace fjorir {
    using namespace nanogui;

    enum class difficulty {
        Beginner = 0,
        Hard,
        Insane
    };

    class Interface : public Screen
    {
    public:
        explicit Interface(bool init = true);
        Interface(Interface& Engine) = delete;

    private:
        Board* canvas;
        void set_board_actions();
        void set_sidebar();
        void set_board();
        difficulty default_difficulty = difficulty::Beginner;
    };
} // namespace fjorir