#pragma once
#include "MKE/Color.hpp"
#include "MKE/DrawContext.hpp"
#include "MKE/Drawable.hpp"
#include "MKE/Transformable.hpp"

namespace mk {
	namespace Debug {
		void setDebugCollisionDraw(bool draw);
	}

	enum class CollisionType { STATIC, DYNAMIC };

	class WorldEntity;

	class CollisionComponent: public Drawable, public Transformable {
	protected:
		bool m_draw = false;

		Color m_outline_color = Color(200, 100, 125, 175);
		Color m_fill_color    = Color(50, 175, 255, 100);

		WorldEntity* m_parent;

	public:
		explicit CollisionComponent(WorldEntity* parent);
		CollisionComponent(const CollisionComponent& c) = default;

		~CollisionComponent() override                           = default;
		virtual bool contains(const math::Vector2f& point) const = 0;

		bool shouldDraw() const;
		void setDraw(bool draw);
		void draw(RenderTarget& target, DrawContext context) const override;
	};
}  // namespace mk
