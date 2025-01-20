#pragma once
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Event.hpp"
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
		EntityID m_entityId = detail::id_counter();

		bool         m_toKill = false;
		WorldEntity* m_parent{};

		bool m_show = true;

		void cleanEntities(Game& game);

		bool m_called_ready = false;

		// if is_paused == true returns from updating and does not propagate
		bool m_is_paused = false;

	public:
		WorldEntity();

		EntityID getId() const;

		void queueFree();

		const bool& isDying() const;

		bool isPaused() const;

		void setPaused(bool paused);

		void setVisible(bool visible);
		void show();
		void hide();

		// We want to it be ordered to be able to iterate in an ordered way.
		std::map<i64, std::list<std::unique_ptr<WorldEntity>>> m_entity_pool;

		void addParent(WorldEntity* parent);

		WorldEntity* getParent() const;

		template<class T, i64 drawOrder = 0>
		requires std::is_base_of_v<WorldEntity, T>
		T* addChild(Game& game, std::unique_ptr<T> child) {
			child->addParent(this);
			auto my_child = child.get();
			m_entity_pool[drawOrder].push_back(std::move(child));
			if (m_called_ready) m_entity_pool[drawOrder].back()->ready(game);
			return my_child;
		}

		template<class T, i64 drawOrder = 0, class... Args>
		requires std::is_base_of_v<WorldEntity, T> T* addChild(Game& game, Args&&... args) {
			auto new_child = std::make_unique<T>(std::forward<Args>(args)...);
			return addChild<T, drawOrder>(game, std::move(new_child));
		}

		void ready(Game& game) override;

		void free(Game& game) override;


		void update(Game& game, float dt) override;

		void physicsUpdate(Game& game, float dt) override;

		virtual void onReady([[maybe_unused]] Game& game) {}

		virtual void onFree([[maybe_unused]] Game& game) {}

		// Called when REenters the top of the scene stack
		virtual void onReReady([[maybe_unused]] Game& game) {}

		virtual void onUpdate([[maybe_unused]] Game& game, [[maybe_unused]] float dt) {}

		virtual void onPhysicsUpdate([[maybe_unused]] Game& game, [[maybe_unused]] float dt) {}

		virtual void onEvent([[maybe_unused]] Game& game, [[maybe_unused]] const Event& event) {}

		virtual void onDraw(
			[[maybe_unused]] RenderTarget& target,
			[[maybe_unused]] DrawContext   context,
			[[maybe_unused]] const Game&   game
		) const {}

		virtual void event([[maybe_unused]] Game& game, [[maybe_unused]] const Event& event) {
			onEvent(game, event);
			for (const auto& layer: m_entity_pool)
				for (auto& entity: layer.second) entity->event(game, event);
		}

		math::Vector3f getGlobalPosition() const;

		math::Matrix4f getGlobalTransform() const;

		virtual DrawMode getDrawMode() const = 0;

		virtual void beginDraw(RenderTarget& target, const Game& game) const = 0;

		virtual void drawEntity(
			RenderTarget& target,
			DrawContext   context,
			const Game&   game,
			DrawMode      draw_mode,
			bool          began
		) const;
	};

	class WorldEntity2D: public WorldEntity {
	public:
		using WorldEntity::WorldEntity;

		void beginDraw(RenderTarget& target, const Game& game) const override;

		DrawMode getDrawMode() const override;
	};

	class WorldEntity3D: public WorldEntity {
	public:
		using WorldEntity::WorldEntity;

		void beginDraw(RenderTarget& target, const Game& game) const override;

		DrawMode getDrawMode() const override;
	};

	class WorldEntityUI: public WorldEntity {
	public:
		using WorldEntity::WorldEntity;

		void beginDraw(RenderTarget& target, const Game& game) const override;

		DrawMode getDrawMode() const override;
	};
}  // namespace mk
