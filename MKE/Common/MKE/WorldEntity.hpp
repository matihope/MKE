#pragma once
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Event.hpp"
#include "MKE/Math/Matrix.hpp"
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

	template<class WE>
	class WorldEntityBase: public Transformable, public Updatable {
	protected:
		EntityID m_entityId;

		bool m_toKill = false;
		WE*  m_parent;

		bool m_show;

		void cleanEntities() {
			for (auto& layer: m_entity_pool) {
				for (auto it = layer.second.begin(); it != layer.second.end(); it++) {
					WE* entity = it->get();
					if (entity->isDying()) it = layer.second.erase(it);
				}
			}
		}

		bool m_called_ready = false;

	public:
		WorldEntityBase(): m_entityId(detail::id_counter()) {
			m_parent = nullptr;
			m_show   = true;
		}

		EntityID getId() const { return m_entityId; }

		void queueFree() { m_toKill = true; }

		const bool& isDying() const { return m_toKill; }

		// We want to it be ordered to be able to iterate in an ordered way.
		std::map<usize, std::list<std::unique_ptr<WE>>> m_entity_pool;

		void addParent(WE* parent) { m_parent = parent; }

		WE* getParent() { return m_parent; }

		template<class T, unsigned int drawOrder = 0>
		requires std::is_base_of_v<WE, T> T* addChild(Game& game, std::unique_ptr<T> child) {
			child->addParent(this);
			auto my_child = child.get();
			m_entity_pool[drawOrder].push_back(std::move(child));
			if (m_called_ready) m_entity_pool[drawOrder].back()->ready(game);
			return my_child;
		}

		template<class T, unsigned int drawOrder = 0, class... Args>
		requires std::is_base_of_v<WE, T> T* addChild(Game& game, Args&&... args) {
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

		virtual void handleEvent(Game& game, const Event& event) {}

		void show() { m_show = true; }

		void hide() { m_show = false; }

		math::Vector3f getGlobalPosition() const {
			if (m_parent == nullptr) return getPosition();
			return getPosition() + m_parent->getGlobalPosition();
		}

		math::Matrix4f getGlobalTransform() const {
			if (m_parent == nullptr) return getTransform();
			return m_parent->getGlobalTransform() * getTransform();
		}
	};

	class WorldEntity2D: public WorldEntityBase<WorldEntity2D>, public Drawable2D {
	public:
		using WorldEntityBase<WorldEntity2D>::WorldEntityBase;

		~WorldEntity2D() = default;

		void draw2d(const RenderTarget2D& target, DrawContext context) const override;

		virtual void onDraw2d(
			[[maybe_unused]] const RenderTarget2D& target, [[maybe_unused]] DrawContext context
		) const {}
	};

	class WorldEntity3D: public WorldEntityBase<WorldEntity3D>, public Drawable3D {
	public:
		using WorldEntityBase<WorldEntity3D>::WorldEntityBase;

		~WorldEntity3D() = default;

		void draw3d(const RenderTarget3D& target, DrawContext context) const override;

		virtual void onDraw3d(
			[[maybe_unused]] const RenderTarget3D& target, [[maybe_unused]] DrawContext context
		) const {}
	};
}  // namespace mk
