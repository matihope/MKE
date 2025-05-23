#include "CollisionComponent.hpp"
#include "MKE/DrawContext.hpp"

namespace mk {
	namespace Debug {
		bool debugDraw = false;

		void setDebugCollisionDraw(bool draw) { debugDraw = draw; }
	}

	CollisionComponent::CollisionComponent(WorldEntity* parent) {
		m_parent = parent;
		// m_shape_repr.setOutlineColor(m_outline_color);
		// m_shape_repr.setOutlineThickness(1.f);
	}

	void CollisionComponent::draw(RenderTarget&, DrawContext) const {
		if (!m_draw && !Debug::debugDraw) return;
	}

	void CollisionComponent::setDraw(const bool draw) { m_draw = draw; }

	bool CollisionComponent::shouldDraw() const { return m_draw || Debug::debugDraw; }
}  // namespace mk
