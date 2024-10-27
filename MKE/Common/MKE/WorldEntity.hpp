#pragma once
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Event.hpp"
#include "MKE/Math/Matrix.hpp"
#include "MKE/RenderTarget.hpp"
#include "MKE/Transformable.hpp"
#include <MKE/Updatable.hpp>
#include <list>
#include <map>
#include <memory>

namespace mk {
	using EntityID = usize;

	namespace detail {
		EntityID id_counter();
	}

	enum class DrawMode { Mode2D, Mode3D, ModeUI };

	class WorldEntity: public Transformable, public Updatable {
	protected:
		EntityID m_entityId;

		bool         m_toKill = false;
		WorldEntity* m_parent;

		bool m_show;

		void cleanEntities() {
			for (auto& layer: m_entity_pool) {
				for (auto it = layer.second.begin(); it != layer.second.end(); it++) {
					WorldEntity* entity = it->get();
					if (entity->isDying()) it = layer.second.erase(it);
				}
			}
		}

		bool m_called_ready = false;

	public:
		WorldEntity(): m_entityId(detail::id_counter()) {
			m_parent = nullptr;
			m_show   = true;
		}

		EntityID getId() const { return m_entityId; }

		void queueFree() { m_toKill = true; }

		const bool& isDying() const { return m_toKill; }

		// We want to it be ordered to be able to iterate in an ordered way.
		std::map<usize, std::list<std::unique_ptr<WorldEntity>>> m_entity_pool;

		void addParent(WorldEntity* parent) { m_parent = parent; }

		WorldEntity* getParent() { return m_parent; }

		template<class T, unsigned int drawOrder = 0>
		requires std::is_base_of_v<WorldEntity, T>
		T* addChild(Game& game, std::unique_ptr<T> child) {
			child->addParent(this);
			auto my_child = child.get();
			m_entity_pool[drawOrder].push_back(std::move(child));
			if (m_called_ready) m_entity_pool[drawOrder].back()->ready(game);
			return my_child;
		}

		template<class T, unsigned int drawOrder = 0, class... Args>
		requires std::is_base_of_v<WorldEntity, T> T* addChild(Game& game, Args&&... args) {
			auto new_child     = std::make_unique<T>(std::forward<Args>(args)...);
			auto new_child_ptr = new_child.get();
			return addChild<T, drawOrder>(game, std::move(new_child));
		}

		void ready(Game& game) override {
			if (!m_called_ready) {
				m_called_ready = true;
				onReady(game);
				for (const auto& layer: m_entity_pool)
					for (auto& entity: layer.second) entity->ready(game);
			}
		}

		void update(Game& game, float dt) override {
			cleanEntities();
			onUpdate(game, dt);
			for (const auto& layer: m_entity_pool)
				for (auto& entity: layer.second) entity->update(game, dt);
		}

		void physicsUpdate(Game& game, const float dt) override {
			onPhysicsUpdate(game, dt);
			for (const auto& layer: m_entity_pool)
				for (auto& entity: layer.second) entity->physicsUpdate(game, dt);
		}

		virtual void onReady(Game& game) {}

		virtual void onUpdate(Game& game, float dt) {}

		virtual void onPhysicsUpdate(Game& game, float dt) {}

		virtual void onDraw(
			[[maybe_unused]] const RenderTarget& target, [[maybe_unused]] DrawContext context
		) const {}

		virtual void handleEvent(Game& game, const Event& event) {}

		math::Vector3f getGlobalPosition() const {
			if (m_parent == nullptr) return getPosition();
			return getPosition() + m_parent->getGlobalPosition();
		}

		math::Matrix4f getGlobalTransform() const {
			if (m_parent == nullptr) return getTransform();
			return m_parent->getGlobalTransform() * getTransform();
		}

		virtual void beginDraw(const RenderTarget& target, const Game& game) const = 0;

		void drawEntity(const RenderTarget& target, DrawContext context) const {
			if (m_show) {
				DrawContext copied_context(context);
				copied_context.transform *= getTransform();
				onDraw(target, context);

				for (const auto& layer: m_entity_pool)
					for (auto& entity: layer.second) entity->drawEntity(target, copied_context);
			}
		}
	};

	class WorldEntity2D: public WorldEntity {
	public:
		using WorldEntity::WorldEntity;

		~WorldEntity2D() = default;

		void beginDraw(const RenderTarget& target, const Game& game) const;
	};

	class WorldEntity3D: public WorldEntity {
	public:
		using WorldEntity::WorldEntity;

		~WorldEntity3D() = default;

		void beginDraw(const RenderTarget& target, const Game& game) const;
	};
}  // namespace mk
