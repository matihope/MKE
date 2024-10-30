#pragma once

#include "MKE/Input.hpp"
#include "MKE/JsonBridge.hpp"
#include "MKE/Math/Vector.hpp"
#include "MKE/Nodes/GUI/Label.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/ResourceManager.hpp"
#include "MKE/WorldEntity.hpp"
#include "MKE/Clock.hpp"

#include <cstddef>
#include <memory>
#include <queue>
#include <stack>
#include <type_traits>

namespace mk {


	class Game {
		bool   m_run                      = false;
		float  m_delta_time               = 0;
		double m_fps_sum                  = 0.0;
		u32    m_fps_frame_count          = 0;
		bool   m_enable_print_fps         = false;
		float  m_physics_update_call_freq = 1. / 60.;
		float  m_physics_update_counter   = 0.f;
		// sf::Cursor::Type m_current_cursor_type;

		ResourceManager res_man;

		mk::Clock                                m_clock;
		JsonBridge                               m_game_settings;
		mk::RenderWindow                         m_window;
		std::stack<std::unique_ptr<WorldEntity>> m_scene_stack;

		Font*                       m_default_font;
		std::unique_ptr<gui::Label> m_fps_label;
		math::Vector2f              m_mouse_pos;
		Image                       icon;

		// void                                     updateViewportSize();
		void                                     recalculateAvgFps();
		std::queue<std::unique_ptr<WorldEntity>> m_safe_scene_delete_queue;

	public:
		explicit Game(const ResPath& settings);

		void run();
		void draw();
		void update();
		void pollEvents();
		void stop();

		void           setPrintFPS(const bool& printFPS);
		bool           isRunning() const;
		math::Vector2u getWindowSize() const;
		// math::Vector2u getViewportSize();
		// void           setViewportSize(math::Vector2u newSize);
		RenderWindow& getRenderWindow();
		bool          isWindowActive() const;

		template<class T, class... Args>
		T* addScene(Args&&... args) {
			return addScene<T>(std::make_unique<T>(std::forward<Args>(args)...));
		}

		template<class T, class... Args>
		T* addScene(std::unique_ptr<T> new_scene) {
			if (isRunning()) new_scene->ready(*this);
			T* ref = new_scene.get();
			m_scene_stack.push(std::move(new_scene));
			return ref;
		}

		void replaceTopScene(std::unique_ptr<WorldEntity> newScene);

		/**
		 * @brief Immediately replace a top scene, but schedule the old one for safe deletion.
		 */
		void popScene();

		math::Vector2f getMousePos();
		bool           isMousePressed(input::MOUSE button) const;
		bool           isMouseJustPressed(input::MOUSE button) const;

		// const sf::View* getView();
		// void setCamera2DCenterAt(const math::Vector2f& pos);
		// void            setCursor(sf::Cursor::Type type);

		mk::Font* getDefaultFont() const;

		bool isKeyPressed(input::KEY key) const;
		bool isKeyJustPressed(input::KEY key) const;

		ResourceManager& resources() { return res_man; }
	};

}  // namespace mk
