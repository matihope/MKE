#include "Game.hpp"
#include "MKE/Collisions/CollisionComponent/CollisionComponent.hpp"
#include "MKE/Color.hpp"
#include "MKE/Event.hpp"
#include "MKE/Init.hpp"
#include "MKE/Input.hpp"

#include <MKE/ResourceManager.hpp>
#include <memory>

namespace {
	mk::math::Vector2f
		scaleToFit(const mk::math::Vector2f viewSize, const mk::math::Vector2u windowSize) {
		mk::math::Vector2f scale;
		scale.x = viewSize.x / (float) windowSize.x;
		scale.y = viewSize.y / (float) windowSize.y;
		if (scale.x < scale.y) {
			scale.x *= 1 / scale.y;
			scale.y = 1;
		} else {
			scale.y *= 1 / scale.x;
			scale.x = 1;
		}
		return scale;
	}
}

namespace mk {
	void Game::run() {
		m_run = true;

		if (!m_scene_stack.empty()) m_scene_stack.top()->ready(*this);

		while (isRunning()) {
			pollEvents();
			update();
			draw();
		}
	}

	void Game::draw() {
		m_window.clear(clear_color);

		if (!m_scene_stack.empty()) m_scene_stack.top()->beginDraw(m_window, *this);

		if (m_enable_print_fps) m_fps_label->beginDraw(m_window, *this);

		m_window.display();
	}

	void Game::update() {
		m_delta_time = m_clock.restart();
		// m_mouse_pos  = math::Vector2f(
		//     getRenderWindow().mapPixelToCoords(sf::Mouse::getPosition(getRenderWindow()))
		// );

		while (!m_safe_scene_delete_queue.empty()) m_safe_scene_delete_queue.pop();

		if (!m_scene_stack.empty()) {
			m_physics_update_counter += m_delta_time;
			while (m_physics_update_counter >= m_physics_update_call_freq) {
				process_mode = ProcessMode::PHYSICS;
				m_scene_stack.top()->physicsUpdate(*this, m_physics_update_call_freq);
				m_physics_update_counter -= m_physics_update_call_freq;

				// Because pollEvent happens before update, then tick has to be after physicsUpdate
				input_map_physics.tick();
			}
			process_mode = ProcessMode::NORMAL;
			m_scene_stack.top()->update(*this, m_delta_time);
			input_map_normal.tick();
		}

		recalculateAvgFps();
	}

	void Game::recalculateAvgFps() {
		if (m_enable_print_fps) {
			++m_fps_frame_count;
			m_fps_sum += m_delta_time;
			if (m_fps_sum >= 1.f) {
				m_fps_label->setString(std::to_string(m_fps_frame_count));
				m_fps_sum         = 0.f;
				m_fps_frame_count = 0;
			}
		}
	}

	void Game::popScene() {
		if (!m_scene_stack.empty()) {
			m_safe_scene_delete_queue.push(std::move(m_scene_stack.top()));
			m_scene_stack.pop();
		}
	}

	void Game::replaceTopScene(std::unique_ptr<WorldEntity> newScene) {
		popScene();
		addScene(std::move(newScene));
	}

	void Game::pollEvents() {
		mk::Event event{};
		while (m_window.pollEvent(event)) {
			input_map_normal.handleEvent(event);
			input_map_physics.handleEvent(event);

			if (!m_scene_stack.empty()) m_scene_stack.top()->event(*this, event);

			if (event.is<mk::Event::WindowClose>())
				stop();
			else if (auto ev = event.get<mk::Event::KeyPressed>(); ev) {
				switch (ev->key) {
				case input::KEY::GRAVE:
					popScene();
					if (m_scene_stack.empty()) stop();
					break;
				default:
					// skip any other case
					break;
				}
			} else if (auto ev = event.get<mk::Event::WindowResized>(); ev) {
				m_fps_label->setPosition(ev->new_size.type<float>().x - 1.f, 1.f);
				updateViewportSize();
			}
		}
	}

	bool Game::isRunning() const { return m_run; }

	void Game::stop() { m_run = false; }

	void Game::setPrintFPS(const bool& printFPS) { m_enable_print_fps = printFPS; }

	math::Vector2u Game::getWindowSize() const { return math::Vector2u(m_window.getSize()); }

	// math::Vector2u Game::getViewportSize() {
	// 	return math::Vector2u(m_view.getSize());
	// }

	RenderWindow& Game::getRenderWindow() { return m_window; }

	void Game::updateViewportSize() {
		math::Vector2f viewportScale
			= scaleToFit(math::Vector2f(m_view_2d.getSize()), getWindowSize());
		// m_view2d.setViewport(sf::FloatRect(
		// 	math::Vector2f(0.5f - viewportScale.x / 2, 0.5f - viewportScale.y / 2), viewportScale
		// ));
		m_view_2d.setSize(viewportScale);
		m_window.setView2D(m_view_2d);
	}

	// const sf::View* Game::getView() { return &m_view; }

	// void Game::setCameraCenterAt(const sf::Vector2f& pos) {
	// 	m_view.setCenter(pos);
	// 	m_window.setView(m_view);
	// 	// polsrodek
	// 	m_fps_label.setPosition(
	// 		pos - sf::Vector2f(384. / 2, 216. / 2)
	// 		+ sf::Vector2f(
	// 			float(m_game_settings["viewport"]["width"]) - 1,
	// 			1
	// 		)
	// 	);  // a
	// }

	// void Game::setCursor(sf::Cursor::Type type) {
	// 	if (m_current_cursor_type == type) return;
	// 	m_current_cursor_type = type;

	// 	getRenderWindow().setMouseCursor(ResourceManager::get().getSystemCursor(type));
	// }

	// bool Game::isWindowActive() const { return m_window.hasFocus(); }

	// void Game::setViewportSize(mk::math::Vector2u newSize) {
	// 	m_view.reset(math::RectF(0, 0, newSize.x, newSize.y));
	// 	updateViewportSize();
	// 	m_fps_label.setPosition(newSize.x - 1, 1);
	// }

	Game::Game(const ResPath& settingsPath) {
		init::init();

		m_game_settings.load(settingsPath);
		m_window.create(
			m_game_settings["window"]["width"],
			m_game_settings["window"]["height"],
			std::string(m_game_settings["window"]["title"])
		);

		m_window.enableVerticalSync(m_game_settings["window"]["vsync"]);
		// setViewportSize(sf::Vector2u(
		// 	m_game_settings["viewport"]["width"], m_game_settings["viewport"]["height"]
		// ));

		// if (!icon.loadFromFile(ResPath("icon.png").strPath()))
		// 	throw mk::Panic("Cannot find icon.png in assets");

		// m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		setPrintFPS(m_game_settings["debug"]["printFPS"]);
		m_physics_update_call_freq
			= 1.f / float(m_game_settings["engine"]["physicsUpdateCallFreq"]);

		Debug::setDebugCollisionDraw(m_game_settings["debug"]["drawCollisionShapes"]);

		m_default_font = res_man.getFont(std::string(m_game_settings["engine"]["fontPath"]));
		m_default_font->setSmooth(bool(m_game_settings["engine"]["fontSmooth"]));

		m_fps_label = std::make_unique<gui::Label>(m_default_font);
		m_fps_label->setFont(m_default_font);
		m_fps_label->setString("0");
		m_fps_label->setTextSize(32);
		m_fps_label->setAlignment(gui::HAlignment::RIGHT, gui::VAlignment::TOP);
		m_fps_label->setPosition({ (float) getWindowSize().x - 1.f, 1.f });

		// lastly, set cursor
		// m_current_cursor_type = sf::Cursor::Arrow;
		// setCursor(sf::Cursor::Arrow);
	}

	mk::Font* Game::getDefaultFont() const { return m_default_font; }

	void Game::setClearColor(const Color c) { clear_color = c; }

	Color Game::getClearColor() const { return clear_color; }

	bool Game::isKeyPressed(input::KEY key) const { return getInput().isKeyPressed(key); }

	bool Game::isKeyJustPressed(input::KEY key) const { return getInput().isKeyJustPressed(key); }

	bool Game::isMousePressed(input::MOUSE button) const {
		return getInput().isMousePressed(button);
	}

	bool Game::isMouseJustPressed(input::MOUSE button) const {
		return getInput().isMouseJustPressed(button);
	}

	bool Game::isKeyJustReleased(input::KEY key) const { return getInput().isKeyJustReleased(key); }

	bool Game::isMouseJustReleased(input::MOUSE button) const {
		return getInput().isMouseJustReleased(button);
	}

	const GameInput& Game::getInput() const {
		if (process_mode == ProcessMode::NORMAL) return input_map_normal;
		return input_map_physics;
	}

	math::Vector2f Game::getMousePos() const { return getInput().getMousePosition(); }

	math::Vector2f Game::getMouseDelta() const { return getInput().getMouseDelta(); }

}  // namespace mk
