#include "WorldEntity.hpp"
#include "Game.hpp"

namespace mk {

	EntityID detail::id_counter() {
		static EntityID newest_id{};
		return newest_id++;
	}

	void WorldEntity::cleanEntities(Game& game) {
		for (auto& layer: m_entity_pool | std::views::values) {
			for (auto it = layer.begin(); it != layer.end(); ++it) {
				if (WorldEntity* entity = it->get(); entity->isDying()) {
					entity->free(game);
					it = layer.erase(it);
				}
			}
		}
	}

	WorldEntity::WorldEntity(): m_entityId(detail::id_counter()) {
		m_parent = nullptr;
		m_show   = true;
	}

	EntityID WorldEntity::getId() const { return m_entityId; }

	void WorldEntity::queueFree() { m_toKill = true; }

	const bool& WorldEntity::isDying() const { return m_toKill; }

	bool WorldEntity::isPaused() const { return m_is_paused; }

	void WorldEntity::setPaused(bool paused) { m_is_paused = paused; }

	void WorldEntity::addParent(WorldEntity* parent) { m_parent = parent; }

	WorldEntity* WorldEntity::getParent() const { return m_parent; }

	void WorldEntity::ready(Game& game) {
		if (!m_called_ready) {
			m_called_ready = true;
			onReady(game);
			for (const auto& layer: m_entity_pool)
				for (auto& entity: layer.second) entity->ready(game);
		}
	}

	void WorldEntity::free(Game& game) {
		onFree(game);
		for (const auto& layer: m_entity_pool | std::views::values)
			for (auto& entity: layer) entity->free(game);
	}

	void WorldEntity::update(Game& game, float dt) {
		cleanEntities(game);

		if (m_is_paused) return;
		onUpdate(game, dt);
		for (const auto& layer: m_entity_pool | std::views::values)
			for (auto& entity: layer) entity->update(game, dt);
	}

	void WorldEntity::physicsUpdate(Game& game, const float dt) {
		if (m_is_paused) return;
		onPhysicsUpdate(game, dt);
		for (const auto& layer: m_entity_pool | std::views::values)
			for (auto& entity: layer) entity->physicsUpdate(game, dt);
	}

	math::Vector3f WorldEntity::getGlobalPosition() const {
		if (m_parent == nullptr) return getPosition();
		return getPosition() + m_parent->getGlobalPosition();
	}

	math::Matrix4f WorldEntity::getGlobalTransform() const {
		if (m_parent == nullptr) return getTransform();
		return m_parent->getGlobalTransform() * getTransform();
	}

	void WorldEntity::drawEntity(
		RenderTarget&  target,
		DrawContext    context,
		const Game&    game,
		const DrawMode draw_mode,
		const bool     began
	) const {
		// Okay, so this method is totally messy... Probably drawing order should get a refactor.

		if (m_show) {
			if (!began && getDrawMode() == draw_mode) {
				beginDraw(target, game);
			} else {
				if (getDrawMode() == draw_mode) onDraw(target, context, game);

				context.transform *= getTransform();
				for (const auto& layer: m_entity_pool | std::views::values)
					for (auto& entity: layer)
						entity->drawEntity(target, context, game, draw_mode, began);
			}
		}
	}

	DrawMode WorldEntity2D::getDrawMode() const { return DrawMode::Mode2D; }

	DrawMode WorldEntity3D::getDrawMode() const { return DrawMode::Mode3D; }

	DrawMode WorldEntityUI::getDrawMode() const { return DrawMode::ModeUI; }

	void WorldEntityUI::beginDraw(RenderTarget& target, const Game& game) const {
		glDisable(GL_DEPTH_TEST);

		DrawContext context;

		auto [width, height] = target.getSize().vec_data;
		context.camera(0, 0) = 2.f / width;
		context.camera(1, 1) = -2.f / height;
		context.camera(0, 3) = -1;
		context.camera(1, 3) = 1;

		drawEntity(target, context, game, getDrawMode(), true);
	}

	void WorldEntity2D::beginDraw(RenderTarget& target, const Game& game) const {
		glDisable(GL_DEPTH_TEST);

		DrawContext context;
		context.camera = target.getCurrentView2D().getTransform();

		drawEntity(target, context, game, getDrawMode(), true);
		drawEntity(target, context, game, DrawMode::ModeUI, false);
	}

	void WorldEntity3D::beginDraw(RenderTarget& target, const Game& game) const {
		glEnable(GL_DEPTH_TEST);

		DrawContext context;
		context.camera = target.getCurrentView3D().getTransform();

		drawEntity(target, context, game, getDrawMode(), true);
		drawEntity(target, context, game, DrawMode::ModeUI, false);
	}

	void WorldEntity::setVisible(bool visible) { m_show = visible; }
}  // namespace mk
