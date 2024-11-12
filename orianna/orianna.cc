/*
 *   orianna is free software under GPL v3 -- see LICENSE for details.
 */

#include <gui.h>
#include <nanogui/nanogui.h>

int main()
{
    try {
        nanogui::init();
        {
            nanogui::ref<orianna::GUI> app = new orianna::GUI();
            app->dec_ref();
            app->draw_all();
            app->set_visible(true);
            nanogui::mainloop(3 / 60.f * 1000);
        }
        nanogui::shutdown();
    } catch (const std::exception& e) {

        std::string error_msg =
          std::string("Caught a fatal error: ") + std::string(e.what());
#if defined(_WIN32)
        MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
#endif
        return -1;
    }
}