/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include <gui.h>
#include <iostream>
#include <nanogui/nanogui.h>

int
main()
{
    try {
        nanogui::init();

        /* scoped variables */
        {
            nanogui::ref<conect::GUI> app = new conect::GUI();
            app->dec_ref();
            app->draw_all();
            app->set_visible(true);
            nanogui::mainloop(1 / 60.f * 1000);
        }

        nanogui::shutdown();
    } catch (const std::exception& e) {
        std::string error_msg =
          std::string("Caught a fatal error: ") + std::string(e.what());
#if defined(_WIN32)
        MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
#else
        std::cerr << error_msg << std::endl;
#endif
        return -1;
    } catch (...) {
        std::cerr << "Caught an unknown error!" << std::endl;
    }
}