/*
    src/example2.cpp -- C++ version of an example application that shows
    how to use the form helper class. For a Python implementation, see
    '../python/example2.py'.
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.
    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/nanogui.h>
#include <iostream>

using namespace nanogui;


enum class difficulty {
    Beginner = 0,
    Hard,
    Insane
};

std::string PLAYER_1 = "Anonymous";

int main(int argc, char** argv) {
    nanogui::init();

    difficulty default = difficulty::Beginner;

    /* scoped variables */ {
        bool use_gl_4_1 = true;// Set to true to create an OpenGL 4.1 context.
        Screen* screen = nullptr;

        if (use_gl_4_1) {
            // NanoGUI presents many options for you to utilize at your discretion.
            // See include/nanogui/screen.h for what all of these represent.
            screen = new Screen(Vector2i(900, 600), "fjorir [GL 4.1]",
                /* resizable */ false, /* fullscreen */ false,
                /* depth_buffer */ true, /* stencil_buffer */ true,
                /* float_buffer */ false, /* gl_major */ 4,
                /* gl_minor */ 1);
        }
        else {
            screen = new Screen(Vector2i(900, 600), "fjorir");
        }

        bool enabled = true;
        FormHelper* gui = new FormHelper(screen);
        ref<Window> window = gui->add_window(Vector2i(50, 50), "Let's Play!");
        gui->add_group("Options");
        gui->add_button("Reset", []() { std::cout << "Game reset!" << std::endl; });
        gui->add_button("Invite...", []() { std::cout << "Game reset!" << std::endl; });
        gui->add_group("Who's Playing");

        gui->add_variable("Player 1", PLAYER_1)->set_placeholder("(type your name here)");
        gui->add_variable("Player 2", std::string("AI"));

        gui->add_variable("Difficulty", default, enabled)
            ->set_items({ "Beginner", "Hard", "Insane" });

        screen->set_visible(true);
        screen->perform_layout();
        //window->center();

        nanogui::mainloop(-1);
    }

    nanogui::shutdown();
    return 0;
}