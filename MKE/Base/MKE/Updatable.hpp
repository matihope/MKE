#pragma once

namespace mk {
	class Game;

	class Updatable {
	public:
		virtual ~Updatable() = default;

		virtual void update([[maybe_unused]] Game& game, [[maybe_unused]] const float dt) {}

		virtual void physicsUpdate([[maybe_unused]] Game& game, [[maybe_unused]] const float dt) {}

		virtual void ready([[maybe_unused]] Game& game) {}

		virtual void free([[maybe_unused]] Game& game) {}
	};
}  // namespace mk
