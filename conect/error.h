/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <nanogui/messagedialog.h>
#include <nanogui/widget.h>
#include <stdlib.h>
#include <string>

namespace conect {

// Ensures only one message dialog is open in the event loop
static int error_dialog_open = 0;

inline void
error_exit_program_dialog(nanogui::Widget* parent, std::string const& msg)
{
    if (!error_dialog_open) {
        error_dialog_open = 1;
        auto dialog = new nanogui::MessageDialog(
          parent, nanogui::MessageDialog::Type::Warning, "Error", msg);
        dialog->set_callback([&](int result) { std::exit(result); });
    }
}

} // namespace conect