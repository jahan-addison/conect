
#include <board.h>
#include <iostream>
#include <nanogui/messagedialog.h>
#include <ranges>

namespace orianna {

namespace fs = std::filesystem;

namespace {

inline auto
get_resource(std::string_view file, auto const& file_roots)
{
    if (std::filesystem::exists(fs::path(file_roots[0].data()) / file.data()))
        return fs::path(file_roots[0].data()) / file.data();
    if (std::filesystem::exists(fs::path(file_roots[1].data()) / file.data()))
        return fs::path(file_roots[1].data()) / file.data();
    return fs::path("");
};

} // namespace

Board::Image
Board::Resource::load_resource(NVGcontext* ctx,
                               Board::Resource::Type type) const
{
    switch (type) {
        case Type::BOARD: {
            fs::path path = get_resource(board, file_roots);
            if (path.empty())
                throw std::runtime_error(
                  "Board::resource::load_resource(): could not find game board "
                  "image");
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case Type::RED_COIN: {
            auto path = get_resource(red_coin, file_roots);
            if (path.empty())
                throw std::runtime_error("Board::resource::load_resource(): "
                                         "could not find red coin image");
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }
        case Type::BLUE_COIN: {
            auto path = get_resource(blue_coin, file_roots);
            if (path.empty())
                throw std::runtime_error("Board::resource::load_resource(): "
                                         "could not find blue coin image");
            return nvgCreateImage(ctx, path.string().c_str(), 0);
        }

        default:
            throw std::runtime_error(
              "Board::resource::load_resource(): invalid resource");
    }
}

Engine::Color
Board::State::get_in_a_row_same_color_of_four() const
{
    auto [row_c, col_c] =
      std::make_tuple(Engine::Color::NONE, Engine::Color::NONE);
    auto adjacent_r = 0, adjacent_c = 0;
    for (auto const& row : layout) {
        auto cols = std::begin(row);
        if ((row_c == Engine::Color::BLUE or row_c == Engine::Color::RED) and
            row_c == *cols)
            adjacent_r++;
        else if (row_c != *cols)
            adjacent_r = 0;
        row_c = *cols;
        if (adjacent_r > 2) {
            return *cols;
        }
        col_c = Engine::Color::NONE;
        adjacent_c = 0;
        for (auto end = std::end(row); cols != end; cols++) {
            if ((col_c == Engine::Color::BLUE or
                 col_c == Engine::Color::RED) and
                col_c == *cols)
                adjacent_c++;
            if (adjacent_c > 2) {
                return *cols;
            }
            col_c = *cols;
        }
    }
    return Engine::Color::NONE;
}

Engine::Color
Board::State::get_diagonal_same_color_of_four(bool start_left = true) const
{
    const int size = layout.size();
    const int row_size = layout.front().size();

    auto color = Engine::Color::NONE;
    auto last = Engine::Color::NONE;

    int adjacent_l = 0;
    int start_col = 0;
    int start_row = 0;
    int last_row = size - 1;

    int row;

    // forward diagonal search
    if (start_left) {
        while (start_row < size or start_col < row_size) {
            if (start_row < size)
                row = start_row++;
            else {
                row = last_row;
                start_col++;
            }
            last = Engine::Color::NONE;
            adjacent_l = 0;
            for (int col = start_col; col < row_size and row >= 0; col++) {
                if ((layout[row][col] == Engine::Color::BLUE or
                     layout[row][col] == Engine::Color::RED) and
                    last == layout[row][col])
                    adjacent_l++;
                else
                    adjacent_l = 0;
                if (adjacent_l > 2)
                    return last;
                if (layout[row][col] == Engine::Color::BLUE or
                    layout[row][col] == Engine::Color::RED)
                    last = layout[row][col];
                else
                    last = Engine::Color::NONE;
                row--;
            }
        }
    }
    // backward diagonal search
    else {
        start_col = layout.back().size();
        while (start_row < size or start_col >= 0) {
            if (start_row < size)
                row = start_row++;
            else {
                row = last_row;
                start_col--;
            }
            last = Engine::Color::NONE;
            adjacent_l = 0;
            for (int col = start_col; col >= 0 and row >= 0; col--) {
                if ((layout[row][col] == Engine::Color::BLUE or
                     layout[row][col] == Engine::Color::RED) and
                    last == layout[row][col])
                    adjacent_l++;
                else
                    adjacent_l = 0;
                if (adjacent_l > 2)
                    return last;
                if (layout[row][col] == Engine::Color::BLUE or
                    layout[row][col] == Engine::Color::RED)
                    last = layout[row][col];
                else
                    last = Engine::Color::NONE;
                row--;
            }
        }
    }

    return color;
}

Engine::Color
Board::State::is_won() const
{
    auto color = this->get_diagonal_same_color_of_four();
    if (color == Engine::Color::NONE)
        color = this->get_diagonal_same_color_of_four(false);
    if (color == Engine::Color::NONE)
        color = this->get_in_a_row_same_color_of_four();
    return color;
}

bool
Board::State::is_full() const
{
    bool full = true;
    for (auto const& k : layout) {
        for (auto const& j : k)
            if (j != Engine::Color::RED and j != Engine::Color::BLUE)
                full = false;
    }
    return full;
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
    this->state.layout.fill({ Engine::Color::NONE,
                              Engine::Color::NONE,
                              Engine::Color::NONE,
                              Engine::Color::NONE,
                              Engine::Color::NONE,
                              Engine::Color::NONE });

    for (auto& column : m_layout) {
        for (auto& texture : column) {
            if (texture != 0) {
                nvgDeleteImage(ctx, texture);
                texture = 0;
            }
        }
    }
}

bool
Board::add_coin(NVGcontext* ctx, Engine::Column col, Engine::Color color)
{
    auto location = &m_layout[engine->column_to_int(col)];
    auto test = std::ranges::find(location->begin(), location->end(), 0);
    if (test != location->end()) {
        state.layout[engine->column_to_int(col)][test - location->begin()] =
          color;
        if (color == Engine::Color::BLUE) {
            location->operator[](test - location->begin()) =
              res.load_resource(ctx, resource_type::BLUE_COIN);
        } else {
            location->operator[](test - location->begin()) =
              res.load_resource(ctx, resource_type::RED_COIN);
        }
        if (location->operator[](test - location->begin()) == 0)
            throw std::runtime_error(
              "Board::add_coin(): could not load coin texture");

        return true;
    }
    return false;
}

void
Board::draw_coins(NVGcontext* ctx) const
{
    auto x_pos = 0, y_pos = 0;
    for (auto const& column : m_layout) {
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

void
Board::draw(NVGcontext* ctx)
{
    nanogui::Screen* scr = screen();
    if (scr == nullptr)
        throw std::runtime_error(
          "Board::draw(): could not find parent screen!");
    if (m_image == -1)
        m_image = res.load_resource(ctx, resource_type::BOARD);

    if (m_image == 0)
        throw std::runtime_error("Board::draw(): could not load game board");

    NVGpaint img_pattern = nvgImagePattern(
      ctx, m_pos.x(), 0, m_size.x(), m_size.y(), 0.f, m_image, 1.f);

    nvgFillPaint(ctx, img_pattern);

    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
    nvgFill(ctx);

    draw_coins(ctx);
}

} // namespace orianna