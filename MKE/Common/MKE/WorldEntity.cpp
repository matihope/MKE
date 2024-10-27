#include "WorldEntity.hpp"
#include "Game.hpp"

namespace mk {
	EntityID detail::id_counter() {
		static EntityID newest_id{};
		return newest_id++;
	}

	void WorldEntityUI::beginDraw(const RenderTarget& target, const Game& game) const {
		DrawContext context;

		// ...
		// context.camera =

		auto [width, height] = game.getWindowSize().bind();
		context.camera(0, 0) = 2.f / width;
		context.camera(1, 1) = -2.f / height;
		context.camera(0, 3) = -1;
		context.camera(1, 3) = 1;

		drawEntity(target, context, game, getDrawMode());
	}
}  // namespace mk
