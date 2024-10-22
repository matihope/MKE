#pragma once

#include "GUI/Label.hpp"
#include "MKE/Clock.hpp"
#include "MKE/JsonBridge.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/WorldEntity.hpp"
#include "Updatable/Updatable.hpp"
#include "WorldEntity/WorldEntity.hpp"
#include "Math/Vector.hpp"

#include <memory>
#include <queue>
#include <stack>

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

		mk::Clock                                m_clock;
		JsonBridge                               m_game_settings;
		mk::RenderWindow                         m_window;
		std::stack<std::unique_ptr<WorldEntity>> m_scene_stack;
		// sf::View                                 m_view;
		Font*          m_default_font;
		gui::Label     m_fps_label;
		math::Vector2f m_mouse_pos;
		Image          icon;

		void                                     updateViewportSize();
		void                                     recalculateAvgFps();
		std::queue<std::unique_ptr<WorldEntity>> m_safe_scene_delete_queue;

	public:
		explicit Game(const ResPath& settings);
		~Game();

		void run();
		void draw();
		void update();
		void pollEvents();
		void stop();

		void           setPrintFPS(const bool& printFPS);
		bool           isRunning() const;
		math::Vector2u getWindowSize();
		math::Vector2u getViewportSize();
		void           setViewportSize(math::Vector2f newSize);
		void           setViewportSize(math::Vector2u newSize);
		RenderWindow&  getRenderWindow();
		bool           isWindowActive() const;

		void addScene(std::unique_ptr<WorldEntity> newScene);
		void replaceTopScene(std::unique_ptr<WorldEntity> newScene);

		/**
		 * @brief Immediately replace a top scene, but schedule the old one for safe deletion.
		 */
		void popScene();

		math::Vector2f  getMousePos();
		const sf::View* getView();
		void            setCamera2DCenterAt(const sf::Vector2f& pos);
		// void            setCursor(sf::Cursor::Type type);
		sf::Font* getDefaultFont() const;
	};

}  // namespace mk
