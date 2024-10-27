#pragma once

namespace mk {
	class Game;

	class Updatable {
	public:
		virtual void update([[maybe_unused]] Game& game, [[maybe_unused]] const float dt) {}

		virtual void physicsUpdate([[maybe_unused]] Game& game, [[maybe_unused]] const float dt) {}

		virtual void ready([[maybe_unused]] Game& game) {}
	};
}  // namespace mk
