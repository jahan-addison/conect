/*
 *   orianna is free software under GPL v3 -- see LICENSE for details.
 */

#pragma once

#include <nanogui/messagedialog.h>
#include <nanogui/screen.h>
#include <nanogui/widget.h>
#include <stdlib.h>
#include <string>

namespace orianna {

namespace {
// Ensures only one message dialog is open in the event loop
bool error_dialog_open = false;
bool info_dialog_open = false;
}

void error_exit_program_dialog(nanogui::Widget* parent, std::string const& msg)
{
    using nanogui::MessageDialog;
    if (!error_dialog_open) {
        parent->inc_ref();
        error_dialog_open = true;

        nanogui::ref<MessageDialog> dialog = new nanogui::MessageDialog(
          parent, nanogui::MessageDialog::Type::Warning, "Error", msg);
        dialog->set_callback([=](int result) { std::exit(result); });
    }
}

void info_message_dialog(nanogui::Widget* parent,
                         std::string const& msg,
                         std::string const& title = " ")
{
    using nanogui::MessageDialog;
    if (!info_dialog_open) {
        auto* scr = parent->screen();
        scr->inc_ref();

        info_dialog_open = true;

        nanogui::ref<MessageDialog> dialog =
          new MessageDialog(scr, MessageDialog::Type::Information, title, msg);

        dialog->set_callback(
          [&, scr]([[maybe_unused]] int result) { scr->redraw(); });

        info_dialog_open = false;
    }
}

} // namespace orianna