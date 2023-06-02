/*
    conect is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    conect is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with conect.  If not, see <https://www.gnu.org/licenses/>.
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