/*
 *   conect is free software under GPL v3 -- see LICENSE for details.
 */

#include <ai/stratzilla.h>
#include <board.h>
#include <chrono>
#include <error.h>
#include <filesystem>
#include <iostream>
#include <nanogui/messagedialog.h>

#include <nanogui/opengl.h>
#include <nanogui/renderpass.h>
#include <nanogui/screen.h>
#include <ranges>
#include <thread>

namespace conect {

namespace fs = std::filesystem;

inline fs::path
get_resource(std::string_view file,
             std::string_view file_root = board::root_resource_directory)
{
    if (fs::exists(fs::path(file_root.data()) / file.data()))
        return fs::path(file_root.data()) / file.data();
    else
        return fs::path("");
}

resource::image
Board::load_resource(NVGcontext* ctx, resource::type type)
{
    switch (type) {
        case resource::type::board: {
            fs::path path = get_resource(BOARD_RESOURCE_FILE);
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case resource::type::red_coin: {
            auto path = get_resource(BOARD_RED_COIN_RESOURCE);
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case resource::type::blue_coin: {
            auto path = get_resource(BOARD_BLUE_COIN_RESOURCE);
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }

        default:
            error_exit_program_dialog(
              this->parent(), "Board::gui::load_resource(): invalid resource");
    }
    return 0;
}

inline std::pair<float, float>
Board::get_coin_drawing_pos(float x_pos, float y_pos) const noexcept
{
    float xx = m_pos.x() + 55.f + (106.2211f * x_pos);
    float yy = m_pos.y() + m_size.y() - 107.2222f - (75.6222f * y_pos);

    return { xx, yy };
}

void
Board::clear_board(NVGcontext* ctx)
{
    for (auto& row : layout)
        std::fill_n(row.begin(), row.size(), board::color::none);

    for (auto& row : i_layout) {
        for (auto& texture : row) {
            if (texture != 0) {
                nvgDeleteImage(ctx, texture);
                texture = 0;
            }
        }
    }
}

void
Board::print_board() const
{
    unsigned int count = 0;
    for (unsigned int i = 0; i < board::size::col; i++) {
        std::cout << " " << i;
    }
    std::cout << std::endl << "---------------" << std::endl;
    for (unsigned int r = 0; r < board::size::row; r++) {
        for (unsigned int c = 0; c < board::size::col; c++) {
            std::cout << "|";
            switch (layout[board::size::row - r - 1][c]) {
                case board::color::none:
                    std::cout << " ";
                    break;
                case board::color::red:
                    std::cout << "O";
                    count++;
                    break;
                case board::color::blue:
                    std::cout << "X";
                    count++;
                    break;
            }
            if (c + 1 == board::size::col) {
                std::cout << "|";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "---------------" << std::endl;
    std::cout << "Piece count: " << count << std::endl;
    std::cout << std::endl;
}

bool
Board::add_coin(NVGcontext* ctx, board::column column, board::color color)
{
    for (unsigned int r = 0; r < board::size::row; r++) {
        if (layout[r][column] == board::color::none) {
            layout[r][column] = color;
            break;
        }
    }
    // we present the transpose of the layout matrix to the gui
    auto location = &i_layout[column];
    auto available = std::ranges::find(location->begin(), location->end(), 0);
    if (available != location->end()) {
        if (color == board::color::blue) {
            location->operator[](available - location->begin()) =
              load_resource(ctx, resource::type::blue_coin);
        } else {
            location->operator[](available - location->begin()) =
              load_resource(ctx, resource::type::red_coin);
        }
        if (location->operator[](available - location->begin()) == 0)
            error_exit_program_dialog(
              this->parent(), "Board::add_coin(): could not load coin texture");
        return true;
    }
    return false;
}

void
Board::draw_coins(NVGcontext* ctx) const
{
    auto x_pos = 0, y_pos = 0;
    for (auto const& column : i_layout) {
        y_pos = 0;
        for (auto const& texture : column) {
            if (texture != 0) {
                auto col_position = get_coin_drawing_pos(
                  static_cast<float>(x_pos), static_cast<float>(y_pos));
                NVGpaint img_pattern = nvgImagePattern(ctx,
                                                       col_position.first,
                                                       col_position.second,
                                                       72.f,
                                                       72.f,
                                                       0.f,
                                                       texture,
                                                       1.f);

                nvgFillPaint(ctx, img_pattern);

                nvgRect(
                  ctx, col_position.first, col_position.second, 72.f, 72.f);
                nvgFill(ctx);
            }
            y_pos++;
        }
        x_pos++;
    }
}

static int end_dialog_open = 0;

void
Board::draw_end_state(bool ending)
{
    if (this->parent()->visible_recursive() and !end_dialog_open) {
        if (!this->engine->get_engine_state() and ending) {
            auto player = engine->get_current_player();
            this->engine->end_engine_state();
            auto dialog = new nanogui::MessageDialog(
              this->screen(),
              nanogui::MessageDialog::Type::Information,
              "Winner!",
              player->name + " is the winner!");
            dialog->set_callback(
              [&]([[maybe_unused]] int result) { end_dialog_open = 1; });
        }

        if (this->engine->is_full(layout)) {
            this->engine->end_engine_state();
            auto dialog = new nanogui::MessageDialog(
              this->screen(),
              nanogui::MessageDialog::Type::Information,
              " ",
              "The game is a draw!");
            dialog->set_callback(
              [&]([[maybe_unused]] int result) { end_dialog_open = 1; });
        }
    }
}

bool
Board::draw_player_state()
{
    auto player = engine->get_current_player();
    auto ai = engine->ai_factory<ai::ST>(engine, &this->layout);

    if (player == engine->get_player(Engine::players::second) and
        !this->engine->get_engine_state()) {
        auto next_move =
          ai.get_next_move(this->engine->get_current_difficulty());
        // did the last player already win?
        if (!ai.get_next_move_is_winning(player->color)) {
            this->add_coin(
              this->screen()->nvg_context(), next_move, player->color);
            // delay event loop to give feel of "thinking" on easy mode :)
            if (this->engine->get_current_difficulty() ==
                ai::difficulty::beginner)
                nanogui::async(
                  [] { std::this_thread::sleep_for(std::chrono::seconds(1)); });
        }
        // did the ai win just now?
        if (!ai.get_next_move_is_winning(player->color)) {
            engine->set_next_player();
            engine->increment_turn_count();
        }
#if defined(DEBUG)
        canvas->print_board();
#endif
    }
    return ai.get_next_move_is_winning(player->color);
}

void
Board::draw(NVGcontext* ctx)
{
    nanogui::Screen* scr = screen();
    if (scr == nullptr)
        error_exit_program_dialog(
          this->parent(), "Board::draw(): could not find parent screen!");
    if (board == -1)
        board = load_resource(ctx, resource::type::board);

    if (board == 0)
        error_exit_program_dialog(this->parent(),
                                  "Board::draw(): could not load board image");

    NVGpaint img_pattern = nvgImagePattern(
      ctx, m_pos.x(), 0, m_size.x(), m_size.y(), 0.f, board, 1.f);

    nvgFillPaint(ctx, img_pattern);

    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
    nvgFill(ctx);

    draw_coins(ctx);

    if (!end_dialog_open)
        draw_end_state(draw_player_state());
}

} // namespace conect