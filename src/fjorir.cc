#include <fjorir.h>

namespace fjorir {
	void Engine::init() {
		this->set_sidebar();
		this->set_game_board();
		this->set_board_actions();
		this->perform_layout();
		this->set_visible(true);
	}

	void Engine::set_board_actions() {
		this->inc_ref();
		FormHelper* gui = new FormHelper(this);
		ref<Window> window = gui->add_window(Vector2i(0, 150), " ");
		window->set_layout(new GridLayout(Orientation::Horizontal, 7, Alignment::Fill));
		window->set_fixed_size(Vector2i(820, 60));
		window->set_position(Vector2i(250, 50));
		for (auto i = 1; i <= 7; i++) {
			Button* b = window->add<Button>(std::to_string(i), FA_ANGLE_DOUBLE_DOWN);
			b->set_background_color(Color(255, 255, 255, 25));
			b->set_callback([] { std::cout << "pushed!" << std::endl; });
		}
	}
	void Engine::set_sidebar() {
		this->inc_ref();
		FormHelper* gui = new FormHelper(this);
		ref<Window> window = gui->add_window(Vector2i(50, 50), "Let's Play!");
		gui->add_group("Options");
		gui->add_button("Reset", []() { std::cout << "Game reset!" << std::endl; });
		gui->add_button("Invite...", []() { std::cout << "Invite your friend!" << std::endl; });
		gui->add_group("Who's Playing");

		gui->add_variable("Player 1", std::string("Anonymous"))->set_placeholder("(type your name here)");
		gui->add_variable("Player 2", std::string("AI"));

		gui->add_variable("Difficulty", this->default_difficulty, true)
			->set_items({ "Beginner", "Hard", "Insane" });
	}

	void Engine::set_game_board() {
#if defined(_WIN32)
		/// Executable is in the Debug/Release/.. subdirectory
		std::string resources_folder_path("../../../resources");
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

		auto image_window = new Window(this, "Game Baord");
		image_window->set_fixed_size(Vector2i(820, 560));
		image_window->set_position(Vector2i(/*820, 520*/ 250, 120));
		image_window->set_layout(new GroupLayout(3));

		Vector2i size;
		int n = 0;


		ImageHolder texture_data(
			stbi_load((images[0].second + ".png").c_str(), &size.x(), &size.y(), &n, 0),
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

} // namespace fjorir