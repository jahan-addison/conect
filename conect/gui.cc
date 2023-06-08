/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include <ai/stratzilla.h>
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

        b->set_callback([&, this, i] { this->board_move_event<AI::ST>(i); });
    }
}

template<class T>
requires(std::is_base_of_v<AI::IAlgorithm, T>)
void
GUI::board_move_event(int index)
{
    auto player = engine->get_current_player();

    auto ai = T(engine, &canvas->layout);
    auto winning_player = ai.get_next_move_is_winning(player->color);

    this->canvas->add_coin(this->nvg_context(),
                           static_cast<gui::Column>(index),
                           engine->get_current_player()->color);

    if (winning_player) {
        new nanogui::MessageDialog(this,
                                   nanogui::MessageDialog::Type::Information,
                                   "Winner!",
                                   player->name + " is the winner!");
        return;
    }

    if (this->engine->is_full(this->canvas->layout)) {
        new nanogui::MessageDialog(this,
                                   nanogui::MessageDialog::Type::Information,
                                   " ",
                                   "The game is a draw!");
        return;
    }

    engine->set_next_player();
    engine->increment_turn_count();

    if (!winning_player) {
        if (player == engine->get_player(Engine::Players::Second)) {
            auto next_move = ai.minimax_alpha_beta_pruning(
              &canvas->layout, 10, 0 - INT_MAX, INT_MAX, gui::Color::BLUE)[1];
            std::cout << "next move: " << next_move << std::endl;
            if (next_move >= 0) {
                board_move_event<AI::ST>(next_move);
                engine->increment_turn_count();
            }
        }
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

    // gui->add_button("Invite...",
    //                 []() { std::cout << "Invite your friend!" << std::endl;
    //                 });

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
    canvas->clear_board(this->nvg_context());
    canvas->set_background_color({ 100, 100, 100, 255 });
    canvas->set_fixed_size({ 820, 520 });
}
} // namespace conect