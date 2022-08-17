#include <gui.h>

namespace linea
{

GUI::GUI()
    : Screen(Vector2i(1150, 800), "Linea", true, false, true, true, false, 4, 1), engine(std::make_shared<Engine>())

{
    this->set_players();
    this->set_sidebar();
    this->set_board();
    this->set_board_actions();
    this->perform_layout();
    this->set_visible(true);
}

void GUI::set_board_actions()
{
    this->inc_ref();
    FormHelper *gui = new FormHelper(this);
    ref<Window> window = gui->add_window(Vector2i(0, 150), "Game Board");
    window->set_layout(new GridLayout(Orientation::Horizontal, 7, Alignment::Fill));
    window->set_fixed_size(Vector2i(820, 60));
    window->set_position(Vector2i(250, 50));
    for (auto i = 1; i <= 7; i++)
    {
        Button *b = window->add<Button>(std::to_string(i), FA_ANGLE_DOUBLE_DOWN);
        b->set_background_color(Color(255, 255, 255, 35));

        b->set_callback([&, this, i] { this->on_coin_event(i); });
    }
}

void GUI::set_next_player()
{
    if (*current_player_turn == player_1)
        current_player_turn = &player_2;
    else
        current_player_turn = &player_1;
}

void GUI::on_coin_event(int index)
{
    this->canvas->add_coin(this->nvg_context(), static_cast<Engine::Column>(index), current_player_turn->color);

    if (auto check = this->canvas->state.is_won(); check != Engine::Color::NONE)
    {
        if (!winner.has_value())
        {
            if (player_1 == check)
                winner.emplace(player_1);
            else
                winner.emplace(player_2);
        }
    }
    if (winner.has_value())
        new nanogui::MessageDialog(this, nanogui::MessageDialog::Type::Information, "Winner!",
                                   winner->name + " is the winner!");

    if (this->canvas->state.is_full())
        new nanogui::MessageDialog(this, nanogui::MessageDialog::Type::Information, " ", "The game is a draw!");

    set_next_player();
}

void GUI::set_players()
{
    player_1.color = Engine::Color::BLUE;

    player_2.name = "AI";
    player_2.color = Engine::Color::RED;
    player_2.ai = true;
}

void GUI::set_sidebar()
{
    this->inc_ref();
    FormHelper *gui = new FormHelper(this);
    ref<Window> window = gui->add_window(Vector2i(50, 50), "Let's Play!");

    gui->add_group("Options");
    gui->add_button("Reset", [&, this] {
        this->winner = std::nullopt;
        this->engine->clear_coins();
        this->canvas->clear_board(this->nvg_context());
        this->canvas->draw_coins(this->nvg_context());
    });

    // @TODO networking
    gui->add_button("Invite...", []() { std::cout << "Invite your friend!" << std::endl; });

    gui->add_group("Who's Playing?");

    gui->add_variable<std::string>(
        "Player 1", [&](std::string value) { player_1.name = value; }, [&]() { return player_1.name; });

    gui->add_variable("Player 2", player_2.name, false);

    gui->add_variable<Engine::Difficulty>(
           "Difficulty", [&](Engine::Difficulty d) { this->engine->set_current_difficulty(d); },
           [&]() { return this->engine->get_current_difficulty(); })
        ->set_items({"Beginner", "Hard"});
}

void GUI::set_board()
{
    this->inc_ref();
    ref<Window> window = new Window(this, " ");
    window->set_fixed_size(Vector2i(820, 560));
    window->set_position(Vector2i(/*820, 520*/ 250, 120));
    canvas = std::make_shared<Board>(window, engine);
    canvas->set_background_color({100, 100, 100, 255});
    canvas->set_fixed_size({820, 520});

    perform_layout();
}

} // namespace linea