
#include <board.h>
#include <filesystem>

namespace fs = std::filesystem;


namespace fjorir {

    Board::Board(Widget* parent) : Canvas(parent, 1), m_image(-1), m_rotation(0.f) {
        using namespace nanogui;
        this->
            m_shader = new Shader(
                render_pass(),

                "Game Board",

#if defined(NANOGUI_USE_OPENGL)
                // Vertex shader
                R"(#version 330
            uniform mat4 mvp;
            in vec3 position;
            in vec3 color;
            out vec4 frag_color;
            void main() {
                frag_color = vec4(color, 1.0);
                gl_Position = mvp * vec4(position, 1.0);
            })",

                // Fragment shader
                R"(#version 330
            out vec4 color;
            in vec4 frag_color;
            void main() {
                color = frag_color;
            })"
#elif defined(NANOGUI_USE_GLES)
                // Vertex shader
                R"(precision highp float;
            uniform mat4 mvp;
            attribute vec3 position;
            attribute vec3 color;
            varying vec4 frag_color;
            void main() {
                frag_color = vec4(color, 1.0);
                gl_Position = mvp * vec4(position, 1.0);
            })",

                // Fragment shader
                R"(precision highp float;
            varying vec4 frag_color;
            void main() {
                gl_FragColor = frag_color;
            })"
#elif defined(NANOGUI_USE_METAL)
                // Vertex shader
                R"(using namespace metal;

            struct VertexOut {
                float4 position [[position]];
                float4 color;
            };

            vertex VertexOut vertex_main(const device packed_float3 *position,
                                         const device packed_float3 *color,
                                         constant float4x4 &mvp,
                                         uint id [[vertex_id]]) {
                VertexOut vert;
                vert.position = mvp * float4(position[id], 1.f);
                vert.color = float4(color[id], 1.f);
                return vert;
            })",

                /* Fragment shader */
                R"(using namespace metal;

            struct VertexOut {
                float4 position [[position]];
                float4 color;
            };

            fragment float4 fragment_main(VertexOut vert [[stage_in]]) {
                return vert.color;
            })"
#endif
            );

        uint32_t indices[3 * 12] = {
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4,
            4, 0, 3, 3, 7, 4,
            1, 5, 6, 6, 2, 1,
            0, 1, 2, 2, 3, 0,
            7, 6, 5, 5, 4, 7
        };

        float positions[3 * 8] = {
            -1.f, 1.f, 1.f, -1.f, -1.f, 1.f,
            1.f, -1.f, 1.f, 1.f, 1.f, 1.f,
            -1.f, 1.f, -1.f, -1.f, -1.f, -1.f,
            1.f, -1.f, -1.f, 1.f, 1.f, -1.f
        };

        float colors[3 * 8] = {
            0, 1, 1, 0, 0, 1,
            1, 0, 1, 1, 1, 1,
            0, 1, 0, 0, 0, 0,
            1, 0, 0, 1, 1, 0
        };

        m_shader->set_buffer("indices", VariableType::UInt32, { 3 * 12 }, indices);
        m_shader->set_buffer("position", VariableType::Float32, { 8, 3 }, positions);
        m_shader->set_buffer("color", VariableType::Float32, { 8, 3 }, colors);
    }

    Board::~Board() {
    }

    void Board::set_rotation(float rotation) {
        m_rotation = rotation;
    }

    void Board::draw_contents() {

        Matrix4f view = Matrix4f::look_at(
            Vector3f(0, -2, -10),
            Vector3f(0, 0, 0),
            Vector3f(0, 1, 0)
        );

        Matrix4f model = Matrix4f::rotate(
            Vector3f(0, 1, 0),
            (float)glfwGetTime()
        );

        Matrix4f model2 = Matrix4f::rotate(
            Vector3f(1, 0, 0),
            m_rotation
        );

        Matrix4f proj = Matrix4f::perspective(
            float(25 * Pi / 180),
            0.1f,
            20.f,
            m_size.x() / (float)m_size.y()
        );

        Matrix4f mvp = proj * view * model * model2;

        m_shader->set_uniform("mvp", mvp);

        // Draw 12 triangles starting at index 0
        m_shader->begin();
        m_shader->draw_array(Shader::PrimitiveType::Triangle, 0, 12 * 3, true);
        m_shader->end();
    }

    void Board::draw(NVGcontext* ctx) {

        nanogui::Screen* scr = screen();
        if (scr == nullptr)
            throw std::runtime_error("Canvas::draw(): could not find parent screen!");

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

        if (m_render_to_texture) {
            m_render_pass->resize(fbsize);
#if defined(NANOGUI_USE_METAL)
            if (m_render_pass_resolved)
                m_render_pass_resolved->resize(fbsize);
#endif
        }
        else {
            m_render_pass->resize(scr->framebuffer_size());
            m_render_pass->set_viewport(offset, fbsize);
        }

        m_render_pass->begin();
        // https://github.com/memononen/nanovg/blob/master/src/nanovg.h
        if (!std::filesystem::exists(fs::path("../../../resources/fjorir-board-filled.png")))
            throw std::runtime_error("could not find game board image");
        if (m_image == -1)
            m_image = nvgCreateImage(ctx, "../../../resources/fjorir-board-filled.png", 0);
        if (m_image == 0)
            throw std::runtime_error("could not load game board");

        NVGpaint img_pattern = nvgImagePattern(ctx, m_pos.x(), 0, m_size.x(), m_size.y(), 0.f, m_image, 1.f);

        nvgFillPaint(ctx, img_pattern);

        nvgBeginPath(ctx);
        nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
        nvgFill(ctx);

        //if (!std::filesystem::exists(fs::path("../../../resources/circle-red.png")))
        //    throw std::runtime_error("could not find coin image");

        //m_coins.emplace_back(nvgCreateImage(ctx, "../../../resources/circle-red.png", 0));

        //img_pattern = nvgImagePattern(ctx, m_pos.x() + 55.f, m_pos.y() + m_size.y() - 72.f - 35.f, 72.f, 72.f, 0.f, m_coins[0], 1.f);

        //nvgFillPaint(ctx, img_pattern);

        //nvgRect(ctx, m_pos.x() + 55.f, m_pos.y() + m_size.y() - 72.f - 35.f, 72.f, 72.f);
        //nvgFill(ctx);

        //nvgClosePath(ctx);


        //if (m_coins.back() == 0)
        //    throw std::runtime_error("could not load game board");

        //draw_contents();
        m_render_pass->end();

        if (m_draw_border) {
            nvgBeginPath(ctx);
            nvgStrokeWidth(ctx, 1.f);
            nvgStrokeColor(ctx, m_border_color);
            nvgRoundedRect(ctx, m_pos.x() + .5f, m_pos.y() + .5f,
                m_size.x() - 1.f, m_size.y() - 1.f,
                m_theme->m_window_corner_radius);
            nvgStroke(ctx);
        }

        if (m_render_to_texture) {
            nanogui::RenderPass* rp = m_render_pass;
#if defined(NANOGUI_USE_METAL)
            if (m_render_pass_resolved)
                rp = m_render_pass_resolved;
#endif
            rp->blit_to(Vector2i(0, 0), fbsize, scr, offset);
        }
    }

} // namespace fjorir