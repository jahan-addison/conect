#include <gui.h>

namespace fjorir {

GUI::GUI(bool init)
    : Screen(Vector2i(1150, 800),
        "fjorir [GL 4.1]",
        true,
        false,
        true,
        true,
        false,
        4,
        1),
    engine(std::make_shared<Engine>())

{
    this->set_sidebar();
    this->set_board();
    this->set_board_actions();
    this->perform_layout();
    this->set_visible(true);
}

void GUI::set_board_actions()
{
    this->inc_ref();
    FormHelper* gui = new FormHelper(this);
    ref<Window> window = gui->add_window(Vector2i(0, 150), "Game Board");
    window->set_layout(
        new GridLayout(Orientation::Horizontal, 7, Alignment::Fill));
    window->set_fixed_size(Vector2i(820, 60));
    window->set_position(Vector2i(250, 50));
    for (auto i = 1; i <= 7; i++)
    {
        Button* b = window->add<Button>(std::to_string(i), FA_ANGLE_DOUBLE_DOWN);
        b->set_background_color(Color(255, 255, 255, 35));
        b->set_callback([&, this, i]
            {
                this->engine->add_coin(static_cast<Engine::Column>(i));
            });
    }
}

void GUI::set_sidebar()
{
    this->inc_ref();
    FormHelper* gui = new FormHelper(this);
    ref<Window> window = gui->add_window(Vector2i(50, 50), "Let's Play!");
    std::string player_1 = "Anonymous";
    std::string player_2 = "AI";
    gui->add_group("Options");
    gui->add_button("Reset", []() { std::cout << "Game reset!" << std::endl; });
    gui->add_button("Invite...",
        []() { std::cout << "Invite your friend!" << std::endl; });
    gui->add_group("Who's Playing");

    gui->add_variable("Player 1", player_1)
        ->set_placeholder("(type your name here)");
    gui->add_variable("Player 2", player_2);

    gui->add_variable("Difficulty", this->default_difficulty, true)
        ->set_items({ "Beginner", "Hard", "Insane" });
}

void GUI::set_board()
{
    this->inc_ref();
    ref<Window> window = new Window(this, " ");
    window->set_fixed_size(Vector2i(820, 560));
    window->set_position(Vector2i(/*820, 520*/ 250, 120));
    canvas = std::make_shared<Board>(window, engine);
    canvas->set_background_color({ 100, 100, 100, 255 });
    canvas->set_fixed_size({ 820, 520 });

    perform_layout();
}

}  // namespace fjorir