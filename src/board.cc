
#include <board.h>
#include <iostream>
#include <ranges>

namespace fjorir {

namespace fs = std::filesystem;

Board::Image Board::resource::load_resource(NVGcontext* ctx, Board::resource::Type type) const
{
    auto get_resource = [&](std::string_view file)
    {
        if (std::filesystem::exists(fs::path(root[0].data()) / file.data()))
            return fs::path(root[0].data()) / file.data();
        if (std::filesystem::exists(fs::path(root[1].data()) / file.data()))
            return fs::path(root[1].data()) / file.data();
        return fs::path("");
    };

    switch (type)
    {
    case Type::BOARD:
    {
        fs::path path = get_resource(board);
        if (path.empty())
            throw std::runtime_error("Board::resource::load_resource(): could not find game board image");
        return nvgCreateImage(ctx, path.string().c_str(), 0);
    }
    case Type::RED_COIN:
    {
        auto path = get_resource(red_coin);
        if (path.empty())
            throw std::runtime_error("Board::resource::load_resource(): could not find red coin image");
        return nvgCreateImage(ctx, path.string().c_str(), 0);
    }
    case Type::BLUE_COIN:
    {
        auto path = get_resource(blue_coin);
        if (path.empty())
            throw std::runtime_error("Board::resource::load_resource(): could not find blue coin image");
        return nvgCreateImage(ctx, path.string().c_str(), 0);
    }

    default:
        throw std::runtime_error("Board::resource::load_resource(): invalid resource");
    }
}

bool Board::State::is_won() const
{
    std::cout << std::endl;
    for (auto const& k : layout)
    {
        std::cout << std::endl;
        for (auto const& j : k)
            std::cout << " " << j << " ";
    }
    return true;
}

bool Board::State::is_tie() const
{
    std::cout << std::endl;
    for (auto const& k : layout)
    {
        std::cout << std::endl;
        for (auto const& j : k)
            std::cout << " " << j << " ";
    }
    return true;
}

Board::State::Token Board::State::get_state() const
{
    this->is_won();
    return Board::State::Token::PLAY;
}


inline std::pair<float, float> Board::get_coin_drawing_pos(float x_pos, float y_pos)
const noexcept
{
    float xx = m_pos.x() + 55.f + (106.2211f * x_pos);
    float yy = m_pos.y() + m_size.y() - 107.2222f - (75.6222f * y_pos);

    return { xx, yy };
}

bool Board::add_coin(NVGcontext* ctx, Engine::Column col, Engine::Color color)
{
    auto location = &m_layout[engine->column_to_int(col)];
    auto test = std::ranges::find(location->begin(), location->end(), 0);
    if (test != location->end())
    {
        state.layout[engine->column_to_int(col)][test - location->begin()] = color;
        if (color == Engine::Color::BLUE)
        {
            location->operator[](test - location->begin()) = res.load_resource(ctx, resource_type::BLUE_COIN);
        } else
        {
            location->operator[](test - location->begin()) = res.load_resource(ctx, resource_type::RED_COIN);
        }
        if (location->operator[](test - location->begin()) == 0)
            throw std::runtime_error("Board::add_coin(): could not load coin texture");

        return true;
    }
    return false;
}

void Board::draw_coins(NVGcontext* ctx) const
{
    auto x_pos = 0, y_pos = 0;
    for (auto const& column : m_layout)
    {
        y_pos = 0;
        for (auto const& texture : column)
        {
            if (texture != 0)
            {
                auto col_position = get_coin_drawing_pos(static_cast<float>(x_pos),
                    static_cast<float>(y_pos));
                NVGpaint img_pattern =
                    nvgImagePattern(ctx, col_position.first, col_position.second, 72.f,
                        72.f, 0.f, texture, 1.f);

                nvgFillPaint(ctx, img_pattern);

                nvgRect(ctx, col_position.first, col_position.second, 72.f, 72.f);
                nvgFill(ctx);
            }
            y_pos++;
        }
        x_pos++;
    }
}

void Board::draw(NVGcontext* ctx)
{
    nanogui::Screen* scr = screen();
    if (scr == nullptr)
        throw std::runtime_error("Board::draw(): could not find parent screen!");

    float pixel_ratio = scr->pixel_ratio();

    Widget::draw(ctx);

    scr->nvg_flush();

    Vector2i fbsize = m_size;
    Vector2i offset = absolute_position();
    if (m_draw_border)
        fbsize -= 2;

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    if (m_render_to_texture)
        offset = Vector2i(offset.x(), scr->size().y() - offset.y() - m_size.y());
#endif

    if (m_draw_border)
        offset += Vector2i(1, 1);

    fbsize = Vector2i(Vector2f(fbsize) * pixel_ratio);
    offset = Vector2i(Vector2f(offset) * pixel_ratio);

    if (m_render_to_texture)
    {
        m_render_pass->resize(fbsize);
#if defined(NANOGUI_USE_METAL)
        if (m_render_pass_resolved)
            m_render_pass_resolved->resize(fbsize);
#endif
    } else
    {
        m_render_pass->resize(scr->framebuffer_size());
        m_render_pass->set_viewport(offset, fbsize);
    }

    m_render_pass->begin();

    if (m_image == -1)
        m_image = res.load_resource(ctx, resource_type::BOARD);

    if (m_image == 0)
        throw std::runtime_error("Board::draw(): could not load game board");

    NVGpaint img_pattern = nvgImagePattern(ctx, m_pos.x(), 0, m_size.x(),
        m_size.y(), 0.f, m_image, 1.f);

    nvgFillPaint(ctx, img_pattern);

    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
    nvgFill(ctx);

    if (this->engine->get_is_receiving())
    {
        auto coin = this->engine->pop_coin();
        add_coin(ctx, coin.second, coin.first);
        state.get_state();
    }

    draw_coins(ctx);

    m_render_pass->end();

    if (m_draw_border)
    {
        nvgBeginPath(ctx);
        nvgStrokeWidth(ctx, 1.f);
        nvgStrokeColor(ctx, m_border_color);
        nvgRoundedRect(ctx, m_pos.x() + .5f, m_pos.y() + .5f, m_size.x() - 1.f,
            m_size.y() - 1.f, m_theme->m_window_corner_radius);
        nvgStroke(ctx);
    }

    if (m_render_to_texture)
    {
        nanogui::RenderPass* rp = m_render_pass;
#if defined(NANOGUI_USE_METAL)
        if (m_render_pass_resolved)
            rp = m_render_pass_resolved;
#endif
        rp->blit_to(Vector2i(0, 0), fbsize, scr, offset);
    }
}

}  // namespace fjorir