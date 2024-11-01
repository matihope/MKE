#include "WorldEntity.hpp"
#include "Game.hpp"

namespace mk {

	EntityID detail::id_counter() {
		static EntityID newest_id{};
		return newest_id++;
	}

	void WorldEntity::cleanEntities() {
		for (auto& layer: m_entity_pool) {
			for (auto it = layer.second.begin(); it != layer.second.end(); it++) {
				WorldEntity* entity = it->get();
				if (entity->isDying()) it = layer.second.erase(it);
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

	void WorldEntity::addParent(WorldEntity* parent) { m_parent = parent; }

	WorldEntity* WorldEntity::getParent() { return m_parent; }

	void WorldEntity::ready(Game& game) {
		if (!m_called_ready) {
			m_called_ready = true;
			onReady(game);
			for (const auto& layer: m_entity_pool)
				for (auto& entity: layer.second) entity->ready(game);
		}
	}

	void WorldEntity::update(Game& game, float dt) {
		cleanEntities();
		onUpdate(game, dt);
		for (const auto& layer: m_entity_pool)
			for (auto& entity: layer.second) entity->update(game, dt);
	}

	void WorldEntity::physicsUpdate(Game& game, const float dt) {
		onPhysicsUpdate(game, dt);
		for (const auto& layer: m_entity_pool)
			for (auto& entity: layer.second) entity->physicsUpdate(game, dt);
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
		RenderTarget& target, DrawContext context, const Game& game, DrawMode draw_mode
	) const {
		if (m_show) {
			if (auto new_mode = getDrawMode();
			    draw_mode != new_mode && new_mode == DrawMode::ModeUI) {
				beginDraw(target, game);
			} else {
				DrawContext copied_context(context);
				copied_context.transform *= getTransform();
				onDraw(target, context, game);

				for (const auto& layer: m_entity_pool)
					for (auto& entity: layer.second)
						entity->drawEntity(target, copied_context, game, draw_mode);
			}
		}
	}

	DrawMode WorldEntity2D::getDrawMode() const { return DrawMode::Mode2D; }

	DrawMode WorldEntity3D::getDrawMode() const { return DrawMode::Mode3D; }

	DrawMode WorldEntityUI::getDrawMode() const { return DrawMode::ModeUI; }

	void WorldEntityUI::beginDraw(RenderTarget& target, const Game& game) const {
		glDisable(GL_DEPTH_TEST);

		DrawContext context;

		auto [width, height] = target.getSize().bind();
		context.camera(0, 0) = 2.f / width;
		context.camera(1, 1) = -2.f / height;
		context.camera(0, 3) = -1;
		context.camera(1, 3) = 1;

		drawEntity(target, context, game, getDrawMode());
	}

	void WorldEntity2D::beginDraw(RenderTarget& target, const Game& game) const {
		glDisable(GL_DEPTH_TEST);

		DrawContext context;
		context.camera = target.getCurrentView2D().getTransform();

		drawEntity(target, context, game, getDrawMode());
	}

	void WorldEntity3D::beginDraw(RenderTarget& target, const Game& game) const {
		glEnable(GL_DEPTH_TEST);

		DrawContext context;
		context.camera = target.getCurrentView3D().getTransform();

		drawEntity(target, context, game, getDrawMode());
	}
}  // namespace mk
