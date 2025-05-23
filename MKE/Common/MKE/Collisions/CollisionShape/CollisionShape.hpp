#pragma once
#include "MKE/Collisions/CollisionComponent/CollisionComponent.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/WorldEntity.hpp"

namespace mk {
	// class CircleCollision: public CollisionComponent {
	// private:
	// 	float m_radius;

	// public:
	// 	explicit CircleCollision(WorldEntity* parent);
	// 	CircleCollision(WorldEntity* parent, float radius);

	// 	~CircleCollision() override = default;
	// 	void  setRadius(float radius);
	// 	float getRadius() const;
	// 	void  draw(RenderTarget& target, DrawContext context) const override;
	// 	bool  contains(const math::Vector2f& point) const override;
	// };

	class RectCollision: public CollisionComponent {
	private:
		math::Vector2f m_size;

	public:
		explicit RectCollision(WorldEntity* parent);
		RectCollision(WorldEntity* parent, float width, float height);

		~RectCollision() override = default;
		void                  setSize(float width, float height);
		const math::Vector2f& getSize() const;
		void                  draw(RenderTarget& target, DrawContext context) const override;
		bool                  contains(const math::Vector2f& point) const override;
	};
}  // namespace mk
