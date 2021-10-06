

#include <nanogui/opengl.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/tabwidget.h>
#include <nanogui/texture.h>
#include <nanogui/nanogui.h>
#include <iostream>
#include <memory>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#if defined(_MSC_VER)
#  pragma warning (disable: 4505) // don't warn about dead code in stb_image.h
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include <stb_image.h>


using namespace nanogui;


enum class difficulty {
    Beginner = 0,
    Hard,
    Insane
};

std::string PLAYER_1 = "Anonymous";

class GLTexture {
public:
    using handleType = std::unique_ptr<uint8_t[], void(*)(void*)>;
    GLTexture() = default;
    GLTexture(const std::string& textureName)
        : mTextureName(textureName), mTextureId(0) {}

    GLTexture(const std::string& textureName, GLint textureId)
        : mTextureName(textureName), mTextureId(textureId) {}

    GLTexture(const GLTexture& other) = delete;
    GLTexture(GLTexture&& other) noexcept
        : mTextureName(std::move(other.mTextureName)),
        mTextureId(other.mTextureId) {
        other.mTextureId = 0;
    }
    GLTexture& operator=(const GLTexture& other) = delete;
    GLTexture& operator=(GLTexture&& other) noexcept {
        mTextureName = std::move(other.mTextureName);
        std::swap(mTextureId, other.mTextureId);
        return *this;
    }
    ~GLTexture() noexcept {
        if (mTextureId)
            glDeleteTextures(1, &mTextureId);
    }

    GLuint texture() const { return mTextureId; }
    const std::string& textureName() const { return mTextureName; }

    /**
    *  Load a file in memory and create an OpenGL texture.
    *  Returns a handle type (an std::unique_ptr) to the loaded pixels.
    */
    handleType load(const std::string& fileName) {
        if (mTextureId) {
            glDeleteTextures(1, &mTextureId);
            mTextureId = 0;
        }
        int force_channels = 0;
        int w, h, n;
        handleType textureData(stbi_load(fileName.c_str(), &w, &h, &n, force_channels), stbi_image_free);
        if (!textureData)
            throw std::invalid_argument("Could not load texture data from file " + fileName);
        glGenTextures(1, &mTextureId);
        glBindTexture(GL_TEXTURE_2D, mTextureId);
        GLint internalFormat;
        GLint format;
        switch (n) {
        case 1: internalFormat = GL_R8; format = GL_RED; break;
        case 2: internalFormat = GL_RG8; format = GL_RG; break;
        case 3: internalFormat = GL_RGB8; format = GL_RGB; break;
        case 4: internalFormat = GL_RGBA8; format = GL_RGBA; break;
        default: internalFormat = 0; format = 0; break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, textureData.get());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        return textureData;
    }

private:
    std::string mTextureName;
    GLuint mTextureId;
};


class Fjorir_Application : public Screen 
{
public:
    Fjorir_Application() : Screen(Vector2i(1150, 800), "fjorir [GL 4.1]",
        true, false,
        true, true,
        false, 4, 1) {
        this->inc_ref();
        bool enabled = true;
        FormHelper* gui = new FormHelper(this);
        ref<Window> window = gui->add_window(Vector2i(50, 50), "Let's Play!");
        gui->add_group("Options");
        gui->add_button("Reset", []() { std::cout << "Game reset!" << std::endl; });
        gui->add_button("Invite...", []() { std::cout << "Invite your friend!" << std::endl; });
        gui->add_group("Who's Playing");

        gui->add_variable("Player 1", PLAYER_1)->set_placeholder("(type your name here)");
        gui->add_variable("Player 2", std::string("AI"));

        gui->add_variable("Difficulty", this->default_difficulty, enabled)
            ->set_items({ "Beginner", "Hard", "Insane" });

        this->load_game_board();

        this->perform_layout();
        this->set_visible(true);
    }

public:
    void load_game_board() {
#if defined(_WIN32)
        /// Executable is in the Debug/Release/.. subdirectory
        std::string resources_folder_path("./resources");
#else
        std::string resources_folder_path("./resources");
#endif
        std::vector<std::pair<int, std::string>> images;

#if !defined(EMSCRIPTEN)
        try {
            images = load_image_directory(m_nvg_context, resources_folder_path);
        }
        catch (const std::exception& e) {
            std::cerr << "Warning: " << e.what() << std::endl;
        }
#endif

        auto image_window = new Window(this, "Game baord");
        image_window->set_fixed_size(Vector2i(820, 560));
        image_window->set_position(Vector2i(/*820, 520*/ 250, 50));
        image_window->set_layout(new GroupLayout(3));

        Vector2i size;
        int n = 0;


        ImageHolder texture_data(
            stbi_load((images[1].second + ".png").c_str(), &size.x(), &size.y(), &n, 0),
            stbi_image_free);
        assert(n == 4);

        Texture* tex = new Texture(
            Texture::PixelFormat::RGBA,
            Texture::ComponentFormat::UInt8,
            size,
            Texture::InterpolationMode::Trilinear,
            Texture::InterpolationMode::Nearest);

        tex->upload(texture_data.get());

        m_images.emplace_back(tex, std::move(texture_data));

        ImageView* image_view = new ImageView(image_window);
        if (!m_images.empty())
            image_view->set_image(m_images[0].first);
        image_view->set_size(Vector2i(820, 520));
        image_view->center();


        image_view->set_pixel_callback(
            [this](const Vector2i& index, char** out, size_t size) {
                const Texture* texture = m_images[0].first.get();
                uint8_t* data = m_images[0].second.get();
                for (int ch = 0; ch < 4; ++ch) {
                    uint8_t value = data[(index.x() + index.y() * texture->size().x()) * 4 + ch];
                    snprintf(out[ch], size, "%i", (int)value);
                }
            }
        );

    }
private: 
    using ImageHolder = std::unique_ptr<uint8_t[], void(*)(void*)>;
    std::vector<std::pair<ref<Texture>, ImageHolder>> m_images;
    difficulty default_difficulty = difficulty::Beginner;
};



int main(int argc, char** argv) {
    try {
        nanogui::init();

        /* scoped variables */ {
            ref<Fjorir_Application> app = new Fjorir_Application();
            app->dec_ref();
            app->draw_all();
            app->set_visible(true);
            nanogui::mainloop(-1);
        }

        nanogui::shutdown();
    }
    catch (const std::exception& e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
#if defined(_WIN32)
        MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
#else
        std::cerr << error_msg << std::endl;
#endif
        return -1;
    }
    catch (...) {
        std::cerr << "Caught an unknown error!" << std::endl;
    }
}