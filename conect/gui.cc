/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include <gui.h>
#include <iostream>
#include <memory>
#include <string>

namespace conect {

GUI::GUI()
  : Screen(Vector2i(1150, 800), "conect", true, false, true, true, false, 4, 1)
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
    for (auto i = 1; i <= 7; i++) {
        Button* b =
          window->add<Button>(std::to_string(i), FA_ANGLE_DOUBLE_DOWN);
        b->set_background_color(Color(255, 255, 255, 35));

        b->set_callback([&, this, i] { this->on_coin_event(i); });
    }
}

void
GUI::on_coin_event(int index)
{
    this->canvas->add_coin(this->nvg_context(),
                           static_cast<resource::Column>(index),
                           engine->get_current_player()->color);

    auto winning_player = this->engine->is_won();

    if (winning_player.has_value())
        new nanogui::MessageDialog(this,
                                   nanogui::MessageDialog::Type::Information,
                                   "Winner!",
                                   winning_player->name + " is the winner!");

    if (this->engine->is_full(this->canvas->layout))
        new nanogui::MessageDialog(this,
                                   nanogui::MessageDialog::Type::Information,
                                   " ",
                                   "The game is a draw!");

    if (!winning_player.has_value())
        engine->set_next_player();
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

    // @TODO networking
    gui->add_button("Invite...",
                    []() { std::cout << "Invite your friend!" << std::endl; });

    gui->add_group("Who's Playing?");

    gui->add_variable<std::string>(
      "Player 1",
      [&](std::string value) {
          auto player = this->engine->get_player(Engine::Players::First);
          this->engine->set_player_name(player, value);
      },
      [&]() { return this->engine->get_player(Engine::Players::First)->name; });

    gui->add_variable<std::string>(
      "Player 2",
      [&](std::string value) {
          auto player = this->engine->get_player(Engine::Players::Second);
          this->engine->set_player_name(player, value);
      },
      [&]() {
          return this->engine->get_player(Engine::Players::Second)->name;
      });

    gui
      ->add_variable<AI::Difficulty>(
        "Difficulty",
        [&](AI::Difficulty d) { this->engine->set_current_difficulty(d); },
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
    canvas->set_background_color({ 100, 100, 100, 255 });
    canvas->set_fixed_size({ 820, 520 });
}
} // namespace conect