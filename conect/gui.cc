/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include <ai/stratzilla.h>
#include <board.h>
#include <engine.h>
#include <gui.h>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace conect {

using namespace nanogui;

GUI::GUI()
  : Screen(Vector2i(1150, 800), "conect", false, false, true, true, false, 4, 1)
  , engine(std::make_shared<Engine>())

{
    this->set_sidebar();
    this->set_board();
    this->set_board_actions();
    this->perform_layout();
    this->set_visible(true);
}

void
GUI::set_board_actions()
{
    this->inc_ref();
    FormHelper* gui = new FormHelper(this);
    ref<Window> window = gui->add_window(Vector2i(0, 150), "Game Board");
    window->set_layout(
      new GridLayout(Orientation::Horizontal, 7, Alignment::Fill));
    window->set_fixed_size(Vector2i(820, 60));
    window->set_position(Vector2i(250, 50));
    for (board::column column = 0; column < 7; column++) {
        Button* b =
          window->add<Button>(std::to_string(column + 1), FA_ANGLE_DOUBLE_DOWN);
        b->set_background_color(Color(255, 255, 255, 35));

        b->set_callback(
          [&, this, column] { this->board_event<ai::ST>(column); });
    }
}

template<class T>
requires(std::is_base_of_v<ai::IAlgorithm, T>)
void
GUI::board_event(board::column column)
{
    auto player = engine->get_current_player();

    auto ai = T(engine, &canvas->layout);

    state_event(ai.get_next_move_is_winning(player->color));

    if (!ended_) {
        this->canvas->add_coin(
          this->nvg_context(), column, engine->get_current_player()->color);
        this->redraw();
    }
    auto ending = ai.get_next_move_is_winning(player->color);
    state_event(ending);

    if (!ended_ and !ending) {
        engine->set_next_player();
        engine->increment_turn_count();
    }

    canvas->print_board();

    if (!ending) {
        if (player == engine->get_player(Engine::players::second)) {
            auto last_player = *player;
            auto next_move =
              ai.get_next_move(this->engine->get_current_difficulty());
            if (next_move >= 0) {
                board_event<ai::ST>(static_cast<board::column>(next_move));
            }
        }
    }
}

void
GUI::state_event(bool ending)
{
    auto player = engine->get_current_player();

    if (!ended_ and ending) {
        ended_ = true;
        new nanogui::MessageDialog(this,
                                   nanogui::MessageDialog::Type::Information,
                                   "Winner!",
                                   player->name + " is the winner!");
        return;
    }

    if (this->engine->is_full(this->canvas->layout)) {
        ended_ = true;
        new nanogui::MessageDialog(this,
                                   nanogui::MessageDialog::Type::Information,
                                   " ",
                                   "The game is a draw!");
        return;
    }
}

void
GUI::set_sidebar()
{
    this->inc_ref();
    FormHelper* gui = new FormHelper(this);
    ref<Window> window = gui->add_window(Vector2i(50, 50), "Let's Play!");

    gui->add_group("Options");
    gui->add_button("Reset", [&, this] {
        this->canvas->clear_board(this->nvg_context());
        this->canvas->draw_coins(this->nvg_context());
    });

    gui->add_group("Who's Playing?");

    gui->add_variable<std::string>(
      "Player 1",
      [&](std::string value) {
          auto player = this->engine->get_player(Engine::players::first);
          this->engine->set_player_name(player, value);
      },
      [&]() { return this->engine->get_player(Engine::players::first)->name; });

    gui->add_variable<std::string>(
      "Player 2",
      [&](std::string value) {
          auto player = this->engine->get_player(Engine::players::second);
          this->engine->set_player_name(player, value);
      },
      [&]() {
          return this->engine->get_player(Engine::players::second)->name;
      });

    gui
      ->add_variable<ai::difficulty>(
        "Difficulty",
        [&](ai::difficulty d) { this->engine->set_current_difficulty(d); },
        [&]() { return this->engine->get_current_difficulty(); })
      ->set_items({ "Beginner", "Hard" });
}

void
GUI::set_board()
{
    this->inc_ref();
    ref<Window> window = new Window(this, " ");
    window->set_fixed_size(Vector2i(820, 560));
    window->set_position(Vector2i(/*820, 520*/ 250, 120));
    canvas = std::make_shared<Board>(window, engine);
    canvas->clear_board(this->nvg_context());
    canvas->set_background_color({ 100, 100, 100, 255 });
    canvas->set_fixed_size({ 820, 520 });
}
} // namespace conect